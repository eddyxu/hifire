/**
 * \brief		Test suite for HIFIRE
 *
 * \author		Lei Xu <lxu@cse.unl.edu>
 *
 * $Id: suite.cpp 7 2009-08-11 05:00:09Z eddyxu $
 */

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main(int argc, char ** argv)
{
	CppUnit::TextUi::TestRunner runner;
	
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry("hifire_tests");
	runner.addTest( registry.makeTest() );
	runner.run();

	return 0;
};

