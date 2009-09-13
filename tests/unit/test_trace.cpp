/**
 * \brief		Implement test cases for Trace
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: test_trace.cpp 7 2009-08-11 05:00:09Z eddyxu $
 */

#include <stdio.h>

#include "trace.h"
#include "test_trace.h"

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TraceTest , "hifire_tests" );

void TraceTest::setUp() 
{

}

void TraceTest::tearDown() 
{

}

void TraceTest::testSize() 
{
	Trace trace;
	CPPUNIT_ASSERT_EQUAL(size_t(0), trace.size());
	trace.insert(1,2,3,4);
	CPPUNIT_ASSERT_EQUAL(size_t(1), trace.size());
	trace.insert(1,2,3,4);
	CPPUNIT_ASSERT_EQUAL(size_t(2), trace.size());

	for(size_t i = 0; i < Trace::BLOCKSIZE; i++) {
		trace.insert(2,3,4,5);
	}
	CPPUNIT_ASSERT_EQUAL( size_t( Trace::BLOCKSIZE + 2 ), trace.size() );
}

void TraceTest::testIterator() 
{
	Trace trace;
	size_t total_items = 5 * Trace::BLOCKSIZE + 13;

	for(size_t i = 0; i < total_items ; i++) {
		trace.insert(2, 3, 4, 5);
	}

	Trace::iterator it = trace.begin();
	size_t i = 0;
	while(it.hasNext()) {
		i ++;
		it.next();
	}
	CPPUNIT_ASSERT_EQUAL( size_t( total_items ), i );
}


void TraceTest::testUpdateEvents() 
{
	Trace trace;
	for(size_t i = 0; i < 100; i++) {
		trace.insert(i, i * 2, i * 3, i * 4);	
	}
	CPPUNIT_ASSERT_EQUAL(size_t(100), trace.size());
	Trace::iterator it = trace.begin();
	while(it.hasNext()) {
		IOEvent * event = it.next();
		event->trace_time ++;
		event->start_time += 2;
	}

	it = trace.begin();
	int i = 0;
	while(it.hasNext()) {
		IOEvent * event = it.next();
		CPPUNIT_ASSERT_EQUAL( double(i + 1), event->trace_time );
		CPPUNIT_ASSERT_EQUAL( double(2), event->start_time );
		i++;
	}
	CPPUNIT_ASSERT_EQUAL( 100, i );
}

