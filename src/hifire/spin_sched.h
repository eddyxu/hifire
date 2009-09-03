/**
 * \brief		Spin_Sched.h
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id$
 *
 * License: New BSD License
 */

#ifndef _HIFIRE_SPIN_SCHED_H_
#define _HIFIRE_SPIN_SCHED_H_

#include "base_sched.h"

class SpinSched : public BaseSched {
public:
	SpinSched(const Config & config);
	~SpinSched();
	int run(Trace & trace);
private:
	void * spin_worker(void * data);

	Trace & trace_;
};

