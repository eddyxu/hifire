/**
 * \brief		Base scheduler class
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: base_sched.h 24 2009-12-25 01:45:02Z eddyxu $
 *
 * License: New BSD License
 */

#ifndef _HIFIRE_SCHEDULER_H_
#define _HIFIRE_SCHEDULER_H_

#include <vector>
#include <sys/types.h>

#include <pthread.h>
#include "sysinfo.h"

class Config;
class Device;
class MTEventQueue;
class Trace;

/**
 * Base scheduler
 */
class BaseSched {
public:
	/**
	 * \brief Constructor
	 */
	BaseSched(const Config & config);

	/**
	 * Deconstruct
	 */
	virtual ~BaseSched();

	virtual int run(Trace & trace) = 0;

	/// stop the scheduler
	virtual void stop() { running_ = false; }
protected:
	typedef std::vector<pthread_t *> threads_type;
	/// get buffer
	void * buffer() { return buffer_; }

	/// get dev fd
	int devfd() { return devfd_; }

	/// get start time of scheduler
	double start_time() { return start_time_; }

	/// set start time of scheduler
	void start_time(double st) { start_time_ = st; }

	bool running() { return running_; }

	MTEventQueue * queue() { return queue_; }

	Device * device() { return device_; }

	int create_threads(void * thread_func(void *) = worker);

	int join_threads();

	static void * worker(void * data);

	static void * batch_worker(void * data);

	threads_type threads_;

	/** 
	 * Spin wait until trace_time is satisfied
	 *
	 * \return  actual start time
	 */
	void wait_until(double trace_time, double pre_spin) { 
		double cur_time;
		double spin_time = trace_time - pre_spin ;
		while(1) {
			cur_time = SysInfo::cur_time() - start_time();
			if( cur_time > spin_time )
				break;
		}
	}
private:
	void *buffer_;
	int devfd_;
	double start_time_;
	const Config & config_;
	Device * device_;
	bool running_; 
	MTEventQueue * queue_;
	size_t thread_num_;
};

#endif /* _HIFIRE_SCHEDULER_H_ */

