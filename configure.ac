#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.66])
AC_INIT([libpo6], [0.2.2], [escriva@cs.cornell.edu])
AM_INIT_AUTOMAKE([foreign dist-bzip2 dist-lzma])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([include/po6/error.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
GTEST_LIB_CHECK

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netinet/in.h limits.h sys/socket.h])

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
AC_CHECK_FUNCS([memmove memset socket])

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 test/Makefile
                 libpo6.pc])
AC_OUTPUT
