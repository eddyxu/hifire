# Makefile.am for hifire source

LIBSRC = \
		 afap_sched.cpp afap_sched.h \
		 base_sched.cpp  base_sched.h \
		 configure.cpp configure.h \
		 device.cpp device.h \
		 sysinfo.cpp sysinfo.h \
		 thread_pool_sched.cpp thread_pool_sched.h \
		 trace.cpp trace.h 

AM_CXXFLAGS = -O3 -DNDEBUG -Wall 
bin_PROGRAMS = hifire
hifire_SOURCES = hifire.cpp hifire.h ${LIBSRC} 



