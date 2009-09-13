/**
 * \brief		Implement test cases for Config
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: test_config.cpp 7 2009-08-11 05:00:09Z eddyxu $
 */

#include <stdio.h>
#include <string>

#include "configure.h"
#include "test_config.h"

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ConfigTest , "hifire_tests" );

using std::string;

void ConfigTest::setUp() 
{

}

void ConfigTest::tearDown() 
{

}

void ConfigTest::testParseArgs() 
{
	char * args[] = { "./hifire", "-o", "some_result_file", 
		"-f", "some_input_file", "-p", "afap", "-z", "13", 
		"-i", "2.12", "-r", "3.03", 
		"--max-time", "1213", "--max-operations", "2345", "/dev/sda"
	};
	Config config;
	CPPUNIT_ASSERT_EQUAL(0, config.parse_args(18, args));
	CPPUNIT_ASSERT_EQUAL(string("some_result_file"), config.result_file());
	CPPUNIT_ASSERT_EQUAL(string("some_input_file"), config.trace_file());
	CPPUNIT_ASSERT_EQUAL(int(Config::POLICY_AFAP), config.policy());
	CPPUNIT_ASSERT_EQUAL(size_t(13), config.thread_num());
	CPPUNIT_ASSERT_EQUAL(2.12, config.time_scale());
	CPPUNIT_ASSERT_EQUAL(3.03, config.range_scale());
	CPPUNIT_ASSERT_EQUAL(string("/dev/sda"), config.dev_path());
}

void ConfigTest::testDefaultValue() 
{
	Config config;
	CPPUNIT_ASSERT_EQUAL(string(), config.trace_file());
	CPPUNIT_ASSERT_EQUAL(string("result.log"), config.result_file());
	CPPUNIT_ASSERT_EQUAL(size_t(4), config.thread_num());
	CPPUNIT_ASSERT_EQUAL(1.0, config.time_scale());
	CPPUNIT_ASSERT_EQUAL(1.0, config.range_scale());
	CPPUNIT_ASSERT_EQUAL(int(Config::POLICY_THREAD_POOL), config.policy());
	CPPUNIT_ASSERT_EQUAL(int(Config::DEV_HARDDRIVE), config.dev_type());
}
