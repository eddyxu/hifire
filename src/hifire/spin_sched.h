/**
 * \brief		Spin_Sched.h
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: spin_sched.h 24 2009-12-25 01:45:02Z eddyxu $
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
	static void * spin_worker(void * data);

	Trace * trace_;
};

#endif /* _HIFIRE_SPIN_SCHED_H_ */
