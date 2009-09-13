# Makefile.am for hifire unit test
#
# $Id

AM_LDFLAGS = -lcppunit -lboost_program_options-mt
AM_CXXFLAGS = -I../../src/hifire

noinst_LIBRARIES = libhifire.a
libhifire_a_SOURCES = \
					../../src/hifire/device.cpp \
					../../src/hifire/trace.cpp \
					../../src/hifire/configure.cpp 

check_PROGRAMS = suite
suite_SOURCES = suite.cpp \
				utils.h utils.cpp \
				test_device.cpp test_device.h \
				test_trace.cpp test_trace.h \
				test_config.cpp test_config.h 

suite_LDADD = libhifire.a

check:
	./suite
