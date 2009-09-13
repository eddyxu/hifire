/**
 * \brief		Test cases for class Config
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: test_config.h 7 2009-08-11 05:00:09Z eddyxu $
 */

#ifndef _HIFIRE_TEST_CONFIG_H_
#define _HIFIRE_TEST_CONFIG_H_

#include "cppunit/TestFixture.h"
#include "cppunit/TestAssert.h"
#include "cppunit/extensions/HelperMacros.h"

class ConfigTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( ConfigTest );
	CPPUNIT_TEST( testParseArgs );
	CPPUNIT_TEST( testDefaultValue );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testParseArgs();
	void testDefaultValue();
};

#endif /* _HIFIRE_TEST_CONFIG_H_ */
