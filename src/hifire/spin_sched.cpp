/**
 * \brief		Spin scheduler
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: spin_sched.cpp 25 2010-01-28 22:02:41Z eddyxu $
 */

#include <stdio.h>
#include <assert.h>
#include "config.h"
#include "trace.h"
#include "spin_sched.h"
#include "device.h"

SpinSched::SpinSched(const Config & config) : BaseSched(config)
{

}


SpinSched::~SpinSched()
{

}

int
SpinSched::run(Trace & trace) 
{
	trace_ = &trace;

	if( create_threads(SpinSched::spin_worker) < 0 ) {
		perror("SpinSched::run: failed to create threads.");
		return -1;
	}

	start_time( SysInfo::cur_time() );

	if( join_threads() < 0 ) {
		perror("SpinSched::run: failed to join threads.");
		return -1;
	}

	return 0;
}

void *
SpinSched::spin_worker(void * data) 
{
	assert(data);
	SpinSched * sched = static_cast<SpinSched *>(data);
	Device * device = sched->device();
	double pre_spin = Config::instance()->pre_spin();
	Trace::iterator iter = sched->trace_->begin();

	while( sched->running() && iter.hasNext() ) {
		IOEvent * event = iter.next();
		sched->wait_until( event->trace_time, pre_spin );

		event->start_time = SysInfo::cur_time() - sched->start_time();
		if(event->opcode == 'W')
			device->pwrite(sched->devfd(), sched->buffer(), event->length, event->offset);
		else
			device->pread(sched->devfd(), sched->buffer(), event->length, event->offset);
		event->end_time = SysInfo::cur_time() - sched->start_time();
	}
	return NULL;
}
