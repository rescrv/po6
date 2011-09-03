#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.66])
AC_INIT([po6], [0.1.dev], [escriva@cs.cornell.edu])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([include/po6/error.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
GTEST_LIB_CHECK

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h sys/socket.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_STRERROR_R
AC_FUNC_STRNLEN
AC_CHECK_FUNCS([memmove memset socket])

AC_CONFIG_FILES([Makefile include/Makefile test/Makefile])
AC_OUTPUT
