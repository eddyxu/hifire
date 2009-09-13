/**
 * \brief		Implement test cases for Device 
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: test_device.cpp 17 2009-08-14 02:36:37Z eddyxu $
 */

#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "device.h"
#include "test_device.h"
#include "utils.h"

using namespace std;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( DeviceTest , "hifire_tests" );

void DeviceTest::setUp() 
{

}

void DeviceTest::tearDown() 
{

}

void DeviceTest::testRaidDevice() 
{
	char filename[] = "hifire-test.XXXXXX";
	int fd = mkstemp(filename);
	close(open(filename, O_CREAT | O_RDWR , S_IRWXU));
	RaidDevice raid(filename);
	CPPUNIT_ASSERT_EQUAL( 0, raid.open() );
	char readbuf[256];
	raid.pwrite(fd, "123456", 7, 0);
	raid.pread(fd, readbuf, 7, 0);
	CPPUNIT_ASSERT_EQUAL( string("123456"), string(readbuf) );

	CPPUNIT_ASSERT_EQUAL( 0, raid.close() );
	unlink(filename);
}

