#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# $Id: configure.ac 22 2009-08-19 21:46:38Z eddyxu $

AC_PREREQ(2.61)
AC_INIT(hifire, 0.1.0, lxu@cse.unl.edu)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/hifire/hifire.cpp])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([cppunit], [main])
AC_CHECK_LIB([boost_program_options-mt], [main], ,
		[AC_CHECK_LIB([boost_program_options], [main], ,AC_MSG_ERROR("boost_program_options not found"))])
AC_CHECK_LIB([boost_thread-mt], [main], , 
		[AC_CHECK_LIB([boost_thread], [main], ,AC_MSG_ERROR("boost_thread not found"))])

# Checks for header files.
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([fcntl.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_CHECK_FUNCS([strstr])
AC_FUNC_STAT
AC_FUNC_ERROR_AT_LINE

AC_CONFIG_FILES([Makefile
		doc/Makefile
		doc/man/Makefile
		src/Makefile
		src/hifire/Makefile
		tests/Makefile
		tests/unit/Makefile
		])
AC_OUTPUT
