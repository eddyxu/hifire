/**
 * \brief		HiFiRe
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: hifire.cpp 25 2010-01-28 22:02:41Z eddyxu $
 */

#include <stdio.h>
#include "base_sched.h"
#include "afap_sched.h"
#include "thread_pool_sched.h"
#include "configure.h"
#include "config.h"
#include "trace.h"

int main(int argc, char * argv[])
{
	Config * config = Config::instance();
	if(config->parse_args(argc, argv) < 0)
		return -1;
	
	BaseSched * sched = NULL;
	switch(config->policy()) {
		case Config::POLICY_THREAD_POOL:	
			sched = new ThreadPoolSched(*config);
			break;
		case Config::POLICY_AFAP:
			sched = new AfapSched(*config);
			break;
		default:
			fprintf(stderr, "Unsupported policy.");
			return -1;
	}

	Trace trace;
	trace.load(*config);

	sched->run(trace);
	trace.write_result(config->result_file());
	delete sched;
	return 0;
}
