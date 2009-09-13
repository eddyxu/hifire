/**
 * \brief		Base scheduler class
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: base_sched.h 7 2009-08-11 05:00:09Z eddyxu $
 *
 * License: New BSD License
 */

#ifndef _HIFIRE_SCHEDULER_H_
#define _HIFIRE_SCHEDULER_H_

#include <vector>
#include <sys/types.h>

#include <pthread.h>

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

