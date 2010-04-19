/**
 * \brief		Implementation of thread pool scheduler
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: thread_pool_sched.cpp 25 2010-01-28 22:02:41Z eddyxu $
 */

#include <stdio.h>
#include <vector>
#include <pthread.h>

#include "thread_pool_sched.h"
#include "configure.h"
#include "trace.h"
#include "sysinfo.h"
#include "event_queue.h"
#include "device.h"

using std::vector;

ThreadPoolSched::ThreadPoolSched(const Config & config) : BaseSched( config )
{
}

ThreadPoolSched::~ThreadPoolSched() 
{
}

int
ThreadPoolSched::run(Trace & trace) 
{
	if(create_threads() < 0) {
		perror("ThreadPoolSched::run: failed to create thread.");
		return -1;
	}

	double pre_spin = Config::instance()->pre_spin();
	Trace::iterator iter = trace.begin();
	
	start_time( SysInfo::cur_time() );
	while( iter.hasNext() ) {
		IOEvent * event = iter.next();
		wait_until( event->trace_time, pre_spin );
		//event->insert_time = curr_time;
		queue()->push(event);
	}

	if( join_threads() < 0 ) {
		perror("ThreadPoolSched::run: failed to join threads.");
		return -1;
	}
	return 0;
}

