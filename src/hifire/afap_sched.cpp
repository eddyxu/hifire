/**
 * \brief		As fast as possible scheduler
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: afap_sched.cpp 5 2009-08-11 04:21:14Z eddyxu $
 */

#include <pthread.h>
#include "trace.h"
#include "afap_sched.h"
#include "sysinfo.h"
#include "event_queue.h"


AfapSched::AfapSched(const Config & config) : BaseSched(config) 
{

}

AfapSched::~AfapSched() 
{

}

int
AfapSched::run(Trace & trace) 
{
	if(create_threads() < 0) {
		perror("AfapSched::run: failed to create threads.");
		return -1;
	}

	Trace::iterator iter = trace.begin();
	start_time(SysInfo::cur_time());
	while(iter.hasNext()) {
		IOEvent * event = iter.next();
		//event->insert_time = curr_time;
		queue()->push(event);
	}

	if(join_threads() < 0) {
		perror("AfapSched::run: failed to join threads.");
		return -1;
	}
	return 0;
}
