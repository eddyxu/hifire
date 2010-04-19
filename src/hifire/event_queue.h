/**
 * \brief	Thread safe event queue
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: event_queue.h 24 2009-12-25 01:45:02Z eddyxu $
 */

#ifndef _HIFIRE_EVENT_QUEUE_H_
#define _HIFIRE_EVENT_QUEUE_H_

#include <queue>
#if defined(WIN32)
#include <windows.h>
#else
#include <pthread.h>
#endif /* unix, linux */

#include <boost/thread.hpp>

using std::queue;
class IOEvent;

/**
 * A base event queue
 */
class Queue {
protected:

	typedef std::queue<IOEvent *> queue_type;

public:

	typedef queue_type::size_type size_type;

	Queue() : done_(false) 
	{
	}

	virtual ~Queue() 
	{
	}

	/// set done flag if there is no more event
	void set_done() 
	{ 
		done_ = true; 
	}

	bool done() 
	{ 
		return done_; 
	}

	virtual void push(IOEvent *) = 0;

	virtual IOEvent * pop() = 0;

	size_type size() 
	{ 
		return events_.size(); 
	}

	bool empty() 
	{ 
		return events_.empty(); 
	}

protected:
	queue_type events_;
private:
	bool done_;
};

/**
 * An event queue without lock
 */
class EventQueue : public Queue {
public:

	EventQueue() 
	{
	}

	virtual ~EventQueue() 
	{
	}

	void push(IOEvent * evt) 
	{ 
		events_.push(evt);
	}

	IOEvent * pop() 
	{
		IOEvent * evt = events_.front();
		events_.pop();
		return evt;
	}
};

/**
 * An event queue w/ lock
 */
class MTEventQueue : public Queue 
{
public:
	MTEventQueue() 
	{
	}

	virtual ~MTEventQueue() 
	{
	}

	IOEvent * pop() 
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		while(empty()){
			cond_.wait(lock);
		}
		IOEvent * v = events_.front();
		events_.pop();
		return v;
	}

	/// pop out all event into a queue
	void pop_all(EventQueue & q) 
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		while(!empty()) 
		{
			cond_.wait(lock);
		}
		
		while(!empty()) 
		{
			IOEvent * evt = events_.front();
			events_.pop();
			q.push(evt);
		}
	}

	void push(IOEvent * v) 
	{
		{
			boost::lock_guard<boost::mutex> lock(mutex_);
			events_.push(v);
		}
		cond_.notify_one();
	}
private:
	boost::condition_variable cond_;
	boost::mutex mutex_;
};

#endif /* _HIFIRE_EVENT_QUEUE_H_ */
