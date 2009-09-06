/**
 * \brief		Provide a device interface to all possible simulators/devices
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: device.h 7 2009-08-11 05:00:09Z eddyxu $
 */

#ifndef _HIFIRE_DEVICE_H_
#define _HIFIRE_DEVICE_H_

#include <string>
#include <sys/types.h>
#include "hifire.h"

#define O_LARGEFILE 0100000 
#if !defined(O_DIRECT)
#define O_DIRECT     040000 
#endif 

/**
 * \class Device
 *
 * Device interface 
 */
class Device {
public:
	/**
	 * \brief Constructor
	 * Construct an device with device path and pre-defined max capacity which can be used
	 * by testing.
	 * If the max capacity is zero, then use the capacity of device to replace it.
	 */
	Device(const string & path, size_t max_capacity = 0);

	/**
	 * \brief Destructor
	 */
	virtual ~Device() {};


	virtual int open() = 0;

	/**
	 * Close device
	 */
	virtual int close() = 0;

	/**
	 * Write device with offset
	 */
	virtual ssize_t pwrite(int fildes, const void *buf, size_t nbyte, off_t offset) = 0;

	/**
	 * Read device with offset
	 */
	virtual ssize_t pread(int d, void *buf, size_t nbyte, off_t offset) = 0;

	/**
	 * Get device capacity
	 */
	size_t capacity() { return capacity_; }

protected:
	string path() { return path_; }

	int fd() { return fd_; }
	void fd(int f) { fd_ = f; }
private:
	string path_;
	size_t capacity_;
	int fd_;
};


/**
 * Raid device
 */
class RaidDevice : public Device {
public:
	RaidDevice(const string & path, size_t max_capacity = 0);
	
	int open();
	int close() { return ::close(fd()); }
	ssize_t pwrite(int fildes, const void *buf, size_t nbyte, off_t offset) {
		return ::pwrite(fildes, buf, nbyte, offset);
	}

	ssize_t pread(int d, void *buf, size_t nbyte, off_t offset) {
		return ::pread(d, buf, nbyte, offset);
	}
};

#endif /* _HIFIRE_DEVICE_H_ */
