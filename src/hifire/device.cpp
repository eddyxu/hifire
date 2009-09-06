/**
 * \brief	 	
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: device.cpp 5 2009-08-11 04:21:14Z eddyxu $
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "hifire.h"
#include "device.h"

Device::Device(const std::string & path, size_t max_capacity) : path_(path) , capacity_(max_capacity) 
{


}


RaidDevice::RaidDevice(const string & path, size_t max_capacity) :
	Device(path, max_capacity)
{ 

}


int
RaidDevice::open()
{ 
	string dev_path = path().c_str();
	if(access(dev_path.c_str(), R_OK)) {
		string errstr = string("RaidDevice::open: can not open ") + dev_path + "!";
		perror(errstr.c_str());
		return -1;
	}

	struct stat buf;
	if(stat(dev_path.c_str(), &buf)) {
		string errstr = string("RaidDevice::open: can not stat device: ") + dev_path + ".";  
		perror(errstr.c_str());
		return -1;
	}

	if(!(S_ISREG(buf.st_mode) || S_ISBLK(buf.st_mode))){
		fprintf(stderr, "RaidDevice::open wrong device type.\n");
		return -1;
	}

	fd(::open(dev_path.c_str(), O_RDWR|O_LARGEFILE|O_DIRECT));
	return 0;
}
