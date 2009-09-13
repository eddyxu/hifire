/**
 * \brief		Test cases for devices
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: test_device.h 7 2009-08-11 05:00:09Z eddyxu $
 */

#ifndef _HIFIRE_TEST_DEVICE_H_
#define _HIFIRE_TEST_DEVICE_H_

#include <vector>
#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>

using std::vector;
using std::string;

class DeviceTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( DeviceTest );
	CPPUNIT_TEST( testRaidDevice );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testRaidDevice();
private:
	vector<string> temp_file_paths;
};

#endif /* _HIFIRE_TEST_DEVICE_H_ */
