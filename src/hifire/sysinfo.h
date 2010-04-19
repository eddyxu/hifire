/**
 * \brief		Get system information
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: sysinfo.h 24 2009-12-25 01:45:02Z eddyxu $
 *
 * License: New BSD License
 *
 * Copyright: 2009 (c) CSE department of University of Nebraska-Lincoln
 */

#ifndef _HIFIRE_SYSINFO_H_
#define _HIFIRE_SYSINFO_H_

#include <string>

/**
 * \class SysInfo
 *
 * Get system information
 */
class SysInfo {
	static double cpufreq_;
public:
	/* 
	 * Initialize System Information class
	 */
	static void init();

	/*
	 * Get CPU Frequency
	 */
	static double cpu_freq();

	/* 
	 * Get total number of CPUs ( active )
	 */
	static size_t cpu_num();
	
	/*
	 * Get current time
	 */
	static double cur_time();
};

#endif /* _HIFIRE_SYSINFO_H_ */
