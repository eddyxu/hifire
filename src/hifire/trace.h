/**
 * \brief		Handle I/O Trace
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: trace.h 24 2009-12-25 01:45:02Z eddyxu $
 */

#ifndef _HIFIRE_TRACE_H_
#define _HIFIRE_TRACE_H_

#include <string>
#include <vector>
#include <list>
#include <assert.h>
#include "configure.h"

using std::list;
using std::string;
using std::vector;

class TraceIterator;

/**
 * I/O Event structure
 */
struct IOEvent {
	IOEvent() : trace_time(0), start_time(0), end_time(0), offset(0), length(0), opcode('W')
	{

	}

	IOEvent(const IOEvent & rhs) : trace_time(rhs.trace_time), start_time(rhs.start_time),
		end_time(rhs.end_time), offset(rhs.offset), length(rhs.length), opcode(rhs.opcode)
	{
	
	}

	IOEvent(double trc_time, off_t off, size_t len, char op) : 
		trace_time(trc_time), start_time(0), end_time(0), offset(off), length(len), opcode(op)
	{

	}

	double trace_time;
	double start_time;
	double end_time;
	off_t offset;
	size_t length;
	char opcode; /* 'R' or 'W' */
};


class EventBlock {
public:
	static const size_t BLOCKSIZE = 512 * 1024; // 512K
	EventBlock(size_t capacity = BLOCKSIZE) : capacity_(capacity), size_(0) {
		events_ = new IOEvent[capacity];
	}

	~EventBlock() { delete[] events_; };

	void push_back(const IOEvent & event) {
		assert(size_ < capacity_);
		events_[size_] = event;	
		size_ ++;
	}
	size_t size() { return size_; }
	bool full() { return size_ == capacity_; }
	IOEvent * begin() { return events_; }
private:
	IOEvent * events_;
	size_t capacity_;
	size_t size_;
};

/**
 * I/O Trace container
 */
class Trace {
	static Trace * instance_;
	friend class TraceIterator;
public:
	static const size_t BLOCKSIZE = 512 * 1024; // 512K
	typedef TraceIterator iterator;
public:
	Trace();
	virtual ~Trace();

	/**
	 * Get singleton instance
	 */
	static Trace * instance();

	/**
	 * Load trace file into memory
	 *
	 * @return 0 for success
	 */
	int load(const Config & conf);

	/**
	 * Insert an I/O event
	 *
	 * \see insert(IOEvent *evt)
	 */
	int insert(double trace_time, off_t offset, size_t length, char opcode);

	/**
	 * Insert an I/O event
	 *
	 * \param evt  an I/O event object
	 */
	int insert(IOEvent * evt);

	/**
	 * Get an I/O event by index
	 */
	IOEvent * get(size_t index) const;

	/**
	 * Get next I/O event
	 */
	IOEvent * next();

	/// get the total number of io events
	size_t size() const;

	/// report trace statistic infomations on screen
	void report() const;

	/// write result to file 
	int write_result(const string & path);

	TraceIterator begin();
private:
	//typedef std::vector<IOEvent> block_t;
	typedef EventBlock block_t ;
	typedef std::list<block_t *> event_ctn_t;
	event_ctn_t events_;
};


class TraceIterator {
public:
	TraceIterator(Trace::event_ctn_t & events);
	TraceIterator(const TraceIterator & it );
	TraceIterator & operator=(const TraceIterator & it );
	inline bool hasNext() { return blk_iter != events.end(); }
	IOEvent * next();
private:
	Trace::event_ctn_t & events;
	Trace::event_ctn_t::const_iterator blk_iter;
	IOEvent * block_base;
	size_t block_idx;
};


#endif /* _HIFIRE_TRACE_H_ */
