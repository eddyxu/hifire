/**
 * \brief		Get system informations
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: sysinfo.cpp 18 2009-08-14 06:13:11Z eddyxu $
 *
 * License: New BSD License
 *
 * Copyright: 2009 (c) CSCE department of University of Nebraska-Lincoln
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include "sysinfo.h"

const size_t BUFSIZE = 1024;

double SysInfo::cpufreq_ = 0;

double SysInfo::cpu_freq() 
{
	if(cpufreq_)
		return cpufreq_;

	FILE *fp;
	char buffer[BUFSIZE];
#if defined(linux) || defined(__linux__)
	if((fp = fopen("/proc/cpuinfo", "r")) == NULL) {
		perror("SysInfo::cpu_freq: Can not get cpu information");
		return 0;
	}

	int i;
	for(i=6; i>0; i--) {
		if(fgets(buffer, BUFSIZE-1, fp) == NULL) {
			perror("SysInfo::cpu_freq: can not get cpu information");
			fclose(fp);
			return 0;
		}
	}

	if(fscanf(fp, "%*s%*s%*s%lf", &cpufreq_) != 1) {
		perror("SysInfo::cpu_freq: can not get cpu information");	
		fclose(fp);
		return 0;
	}
	cpufreq_ *= 1000000;
#elif defined(__APPLE__) || defined(__NetBSD__) || defined(__FreeBSD__)

	if( (fp = popen("sysctl -a", "r")) == NULL) {
		perror("cpu_freq(): get cpu frequency");
		return 0;
	}

	while(fgets(buffer, BUFSIZE-1, fp) != NULL) {
#if defined(__APPLE__)
		if(strnstr(buffer, "hw.cpufrequency:", BUFSIZE) == NULL)
#else
		if(strnstr(buffer, "dev.cpu.0.freq:", BUFSIZE) == NULL)
#endif /* FreeBSD & NetBSD */
			continue;

		sscanf(buffer, "%*s%lf", &cpufreq_);
#if defined(__FreeBSD__) || defined(__NetBSD__)
		cpufreq_ *= 1000000;
#endif /* FreeBSD / NetBSD */ 
		break;
	};
#endif /* __APPLE__ || __FreeBSD__ || __NetBSD__ */
	fclose(fp);
	return cpufreq_;
}


/**
 * Get available number of CPUs 
 *
 * @see http://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
 */
size_t SysInfo::cpu_num() 
{
	long numCPU;
#if defined(linux) || defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
	numCPU = sysconf( _SC_NPROCESSORS_ONLN );
#elif defined(WIN32)
	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );
	numCPU = sysinfo.dwNumberOfProcessors;
#endif 
	return numCPU;
}


void
SysInfo::init()
{
	cpu_freq();
}	

inline uint64_t rdtsc() 
{
	uint32_t lo, hi;
	__asm__ __volatile__
		("rdtsc" : "=a"(lo), "=d"(hi));
	return (uint64_t) hi << 32 | lo;
}

double SysInfo::cur_time(void) 
{
	return rdtsc() / cpufreq_;
}


