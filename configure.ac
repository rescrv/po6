#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([libpo6], [0.8.dev], [robert@hyperdex.org])
m4_define([serial_tests], [
    m4_esyscmd([case `automake --version | head -n 1` in
                *1.11*);;
                *) echo serial-tests;;
                esac])
])
AM_INIT_AUTOMAKE(foreign serial_tests subdir-objects dist-bzip2)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
LT_PREREQ([2.2])
LT_INIT
AC_CONFIG_SRCDIR([po6/error.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

ANAL_WARNINGS

# Checks for libraries.

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
                 libpo6.pc])
AC_OUTPUT
