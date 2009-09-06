/**
 * \brief		As fast as possible scheduler
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: afap_sched.h 7 2009-08-11 05:00:09Z eddyxu $
 */

#ifndef _HIFIRE_AFAP_SCHED_H_
#define _HIFIRE_AFAP_SCHED_H_

#include "base_sched.h"

class AfapSched : public BaseSched {
public:
	AfapSched(const Config & config);
	~AfapSched();

	int run(Trace & trace);
};

#endif /* _HIFIRE_AFAP_SCHED_H_ */
