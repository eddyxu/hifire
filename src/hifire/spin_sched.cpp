/**
 * \brief		Spin scheduler
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id$
 */

#include <assert.h>
#include "config.h"
#include "trace.h"
#include "spin_sched.h"

SpinSched::SpinSched(const Config & config) : BaseSched(config)
{

}


SpinSched::~SpinSched()
{

}

int
SpinSched::run(Trace & trace) 
{
	trace_ = trace;
	return 0;
}

void *
SpinSched::spin_worker(void * data) 
{
	assert(data);
	SpinSched * sched = static_cast<SpinSched *>(data);
	Device * device = sched->device();
	double pre_spin = Config::instance()->pre_spin();
	return NULL;
}
