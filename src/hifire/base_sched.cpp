/**
 * \brief		implement base scheduler
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: base_sched.cpp 5 2009-08-11 04:21:14Z eddyxu $
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "device.h"
#include "config.h"
#include "base_sched.h"
#include "event_queue.h"
#include "trace.h"
#include "sysinfo.h"

BaseSched::BaseSched(const Config & config)
	: buffer_(NULL), devfd_(0), start_time_(0), config_(config), device_(NULL), running_(true)
{
	if(config.dev_type() == Config::DEV_HARDDRIVE) {
		device_ = new RaidDevice(config.dev_path(), config.dev_capacity());
	} else {
		fprintf(stderr, "Unknown device type.\n");
		exit(1);
	}
	devfd_ = device_->open();
	if(devfd_ < 0) {
		perror("Failed to open device.");
		exit(1);
	}
	queue_ = new MTEventQueue();

	thread_num_ = config.thread_num();
	for(size_t i = 0; i < thread_num_ ; i++) {
		threads_.push_back(new pthread_t);
	}
	SysInfo::init();
}


BaseSched::~BaseSched()
{
	device_->close();
	delete(device_);
	free(buffer_);
	delete(queue_);
	for(threads_type::iterator it = threads_.begin(); it != threads_.end(); ++it) {
		delete * it;
	}

}


void * 
BaseSched::worker(void * data) 
{
	assert(data);
	BaseSched * sched = static_cast<BaseSched *>(data);
	Device * device = sched->device();
	while(sched->running()) {
		IOEvent * event = sched->queue()->pop();
		if(!event) {
			break;
		}
		event->start_time = SysInfo::cur_time() - sched->start_time();
		if(event->opcode == 'W')
			device->pwrite(sched->devfd(), sched->buffer(), event->length, event->offset);
		else
			device->pread(sched->devfd(), sched->buffer(), event->length, event->offset);
		event->end_time = SysInfo::cur_time() - sched->start_time();
	};
	return NULL;
}

void *
BaseSched::batch_worker(void * data) 
{
	assert(data);
	BaseSched * sched = static_cast<BaseSched *>(data);
	Device * device = sched->device();
	EventQueue q;
	bool done = false;
	while(sched->running() && !done) {
		sched->queue()->pop_all(q);
		while(!q.empty()) {
			IOEvent * event = q.pop();
			if(!event){
				done = true;
				break;
			}
			event->start_time = SysInfo::cur_time() - sched->start_time();
			if(event->opcode == 'W')
				device->pwrite(sched->devfd(), sched->buffer(), event->length, event->offset);
			else
				device->pread(sched->devfd(), sched->buffer(), event->length, event->offset);
			event->end_time = SysInfo::cur_time() - sched->start_time();
		}
	}

	return NULL;
}

int
BaseSched::create_threads() 
{
	pthread_attr_t child_attr;
	pthread_attr_init(&child_attr);
	struct sched_param param;
	param.sched_priority = 95;
	pthread_attr_setschedparam(&child_attr, &param);
	pthread_attr_setschedpolicy(&child_attr, SCHED_FIFO);

	for(threads_type::iterator it = threads_.begin(); it != threads_.end(); ++it) {
		if(pthread_create(*it, &child_attr, BaseSched::worker, this)) {
			return -1;
		}
	}
	printf("Created %zu threads\n", threads_.size());
	param.sched_priority = 5;
	pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);

	return 0;
}


int
BaseSched::join_threads() 
{
	// TODO: find a better way to terminate all children
	for(size_t i = 0; i < threads_.size(); i++) {
		queue()->push(NULL);
	}

	for(threads_type::iterator it = threads_.begin(); it != threads_.end(); ++it) {
		if(pthread_join(**it, NULL)) {
			return -1;
		}
	}

	return 0;
}
