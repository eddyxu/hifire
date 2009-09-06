/**
 * \brief		Trace Container
 *
 * HIFIRE can take several formats of traces as input source
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: trace.cpp 18 2009-08-14 06:13:11Z eddyxu $
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "trace.h"

using std::vector;
using std::list;
using std::string;

const size_t BUFSIZE = 1024;

/// singleton object
Trace * 
Trace::instance_ = NULL;

Trace *
Trace::instance() 
{
	if(!Trace::instance_) {
		Trace::instance_ = new Trace();
	}
	return Trace::instance_;
}


Trace::Trace() 
{

}


Trace::~Trace() 
{
	for(event_ctn_t::iterator bit = events_.begin(); bit != events_.end(); ++bit) {
		delete (*bit);
	}
}

int
Trace::load(const Config & conf) 
{
	assert(!conf.trace_file().empty());

	FILE *fp;
	if((fp = fopen(conf.trace_file().c_str(), "r")) == NULL) {
		perror("Trace::load: failed to open trace file.");
		return -1;
	}

	char buffer[BUFSIZE];
	int devno;
	size_t address, length, offset;
	char opcode;
	double timestamp;
	double time_scale = Config::instance()->time_scale();
	double range_scale = Config::instance()->range_scale();
	const size_t BLOCKSIZE = 512;

	while(1) {
		if(fgets(buffer, BUFSIZE - 1, fp) == NULL)
			break;

		if (sscanf(buffer, "%d,%zd,%zd,%c,%lf\n",&devno,&address,&length,&opcode,&timestamp) != 5) {
			fprintf(stderr, "devno=%d time=%f op=%c address=%zd length=%zd\n", devno,timestamp,opcode,address,length);
			fprintf(stderr, "Wrong number of arguments for I/O trace event type\n");
			fprintf(stderr, "line: %s", buffer);
			fclose(fp);
			return -1;
		}

		timestamp /= time_scale;
		length *= range_scale;
		offset = address * BLOCKSIZE;
		if(offset + length > conf.dev_capacity())  	
			length = conf.dev_capacity() - offset; // TODO: What if offset > dev_capacity

		if(insert(timestamp, offset, length, opcode) < 0) {
			fclose(fp);
			return -1;
		}
	}

	return 0;
}



int 
Trace::write_result(const string & path) 
{
	Config * config = Config::instance();
	FILE *fp;
	if((fp = fopen(path.c_str(), "w+")) == NULL) {
		perror("Trace::write_result: can not write result file.");
		return -1;
	}

	fprintf(fp, "# trace_time,start_time,end_time,error_time\n");
	iterator it = begin();
	int level = config->output_level();
	assert( level == Config::OUTPUT_FULL || level == Config::OUTPUT_BRIEF );
	while(it.hasNext()) {
		IOEvent * event = it.next();
		if(level == Config::OUTPUT_BRIEF) {
			fprintf(fp, "%lf,%lf,%lf,%lf\n", event->trace_time, event->start_time, 
					event->end_time, event->start_time - event->trace_time);
		} else if (level == Config::OUTPUT_FULL) {
			fprintf(fp, "%lf,%jd,%zd,%lf,%lf,%lf\n", event->trace_time, 
					event->offset, event->length, 
					event->start_time, event->end_time, event->start_time - event->end_time);
		} 
	}

	fclose(fp);
	return 0;
}


int 
Trace::insert(double trace_time, off_t offset, size_t length, char opcode) 
{
	if(events_.empty() || events_.back()->size() >= BLOCKSIZE) {
		block_t * new_blk = new block_t;
		events_.push_back(new_blk);
	}
	block_t * blk = events_.back();
	IOEvent evt = IOEvent(trace_time, offset, length, opcode);
	blk->push_back(evt);
	return 0;
}


int
Trace::insert(IOEvent * evt) 
{
	assert(evt);
	return insert(evt->trace_time, evt->offset, evt->length, evt->opcode);
}


size_t 
Trace::size() const 
{
	if(events_.empty())
		return 0;

	return (events_.size() - 1) * BLOCKSIZE + events_.back()->size();
}

TraceIterator Trace::begin() 
{ 
	return TraceIterator(events_); 
}

TraceIterator::TraceIterator(Trace::event_ctn_t & evts) : 
	events(evts)
{
	blk_iter = events.begin();
	if(hasNext()) {
		block_base = (*blk_iter)->begin();
		block_idx = 0;
	}
}

TraceIterator::TraceIterator(const TraceIterator & rhs) : events(rhs.events)
{
	*this = rhs;
}

TraceIterator & TraceIterator::operator=(const TraceIterator & rhs) 
{
	events = rhs.events;
	blk_iter = events.begin();
	if(hasNext()) {
		block_base = (*blk_iter)->begin();
		block_idx = 0;
	}
	return *this;
}

IOEvent *
TraceIterator::next() 
{
	IOEvent * evt = block_base + block_idx;
	block_idx++;
	if( block_idx == (*blk_iter)->size()) {
		++blk_iter;
		if(hasNext()) {
			block_base = (*blk_iter)->begin();
			block_idx = 0;
		}
	}
	return evt;
}
