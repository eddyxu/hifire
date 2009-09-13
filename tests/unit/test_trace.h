/**
 * \brief		Test cases for Trace
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: test_trace.h 7 2009-08-11 05:00:09Z eddyxu $
 */

#ifndef _HIFIRE_TEST_TRACE_H_
#define _HIFIRE_TEST_TRACE_H_

#include "cppunit/TestFixture.h"
#include "cppunit/TestAssert.h"
#include "cppunit/extensions/HelperMacros.h"

class TraceTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( TraceTest );
	CPPUNIT_TEST( testSize );
	CPPUNIT_TEST( testIterator );
	CPPUNIT_TEST( testUpdateEvents );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testSize();
	void testIterator();
	void testUpdateEvents();
};

#endif /* _HIFIRE_TEST_TRACE_H_ */
