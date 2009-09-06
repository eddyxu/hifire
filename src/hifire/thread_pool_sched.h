/**
 * \brief		Thread pool scheduler
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: thread_pool_sched.h 7 2009-08-11 05:00:09Z eddyxu $
 */

#ifndef _HIFIRE_THREAD_POOL_SCHED_H_
#define _HIFIRE_THREAD_POOL_SCHED_H_

#include <vector>
#include <pthread.h>
#include "base_sched.h"

class Config;
class Trace;

class ThreadPoolSched : public BaseSched {
public:
	ThreadPoolSched(const Config & config);
	~ThreadPoolSched();

	int run(Trace & trace);
private:
	size_t pool_size_;
};

#endif /* _HIFIRE_THREAD_POOL_SCHED_H_ */

