ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS =
AM_CFLAGS = $(WANAL_CFLAGS)
AM_CXXFLAGS = $(WANAL_CXXFLAGS)
if MAKE_NO_PRINT_DIRECTORY
AM_MAKEFLAGS = --no-print-directory
endif

EXTRA_DIST = LICENSE README

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libpo6.pc

nobase_include_HEADERS =
nobase_include_HEADERS += po6/errno.h
nobase_include_HEADERS += po6/io/fd.h
nobase_include_HEADERS += po6/io/mmap.h
nobase_include_HEADERS += po6/net/hostname.h
nobase_include_HEADERS += po6/net/ipaddr.h
nobase_include_HEADERS += po6/net/location.h
nobase_include_HEADERS += po6/net/socket.h
nobase_include_HEADERS += po6/path.h
nobase_include_HEADERS += po6/threads/barrier.h
nobase_include_HEADERS += po6/threads/cond.h
nobase_include_HEADERS += po6/threads/mutex.h
nobase_include_HEADERS += po6/threads/rwlock.h
nobase_include_HEADERS += po6/threads/thread.h
nobase_include_HEADERS += po6/time.h

#################################### Source ####################################

lib_LTLIBRARIES = libpo6.la
libpo6_la_SOURCES =
libpo6_la_SOURCES += barrier.cc
libpo6_la_SOURCES += cond.cc
libpo6_la_SOURCES += errno.cc
libpo6_la_SOURCES += fd.cc
libpo6_la_SOURCES += hostname.cc
libpo6_la_SOURCES += ipaddr.cc
libpo6_la_SOURCES += location.cc
libpo6_la_SOURCES += mmap.cc
libpo6_la_SOURCES += mutex.cc
libpo6_la_SOURCES += path.cc
libpo6_la_SOURCES += rwlock.cc
libpo6_la_SOURCES += socket.cc
libpo6_la_SOURCES += thread.cc
libpo6_la_SOURCES += time.cc
libpo6_la_LIBADD = $(RT_LIBS) -lpthread

##################################### Tests ####################################

th_sources = test/runner.cc th.cc th.h

TESTS = $(check_PROGRAMS)
check_PROGRAMS =
check_PROGRAMS += test/errno
check_PROGRAMS += test/io_fd
check_PROGRAMS += test/net/hostname
check_PROGRAMS += test/net/ipaddr
check_PROGRAMS += test/net/location
check_PROGRAMS += test/net/socket
check_PROGRAMS += test/path
check_PROGRAMS += test/threads/cond
check_PROGRAMS += test/threads/mutex
check_PROGRAMS += test/threads/rwlock
check_PROGRAMS += test/threads/thread
check_PROGRAMS += test/time

test_errno_SOURCES = test/errno.cc $(th_sources)
test_errno_LDADD = libpo6.la

test_io_fd_SOURCES = test/io/fd.cc $(th_sources)
test_io_fd_LDADD = libpo6.la

test_net_hostname_SOURCES = test/net/hostname.cc $(th_sources)
test_net_hostname_LDADD = libpo6.la

test_net_ipaddr_SOURCES = test/net/ipaddr.cc $(th_sources)
test_net_ipaddr_LDADD = libpo6.la

test_net_location_SOURCES = test/net/location.cc $(th_sources)
test_net_location_LDADD = libpo6.la

test_net_socket_SOURCES = test/net/socket.cc $(th_sources)
test_net_socket_LDADD = libpo6.la

test_path_SOURCES = test/path.cc $(th_sources)
test_path_LDADD = libpo6.la

test_threads_cond_SOURCES = test/threads/cond.cc $(th_sources)
test_threads_cond_LDADD = libpo6.la

test_threads_mutex_SOURCES = test/threads/mutex.cc $(th_sources)
test_threads_mutex_LDADD = libpo6.la

test_threads_rwlock_SOURCES = test/threads/rwlock.cc $(th_sources)
test_threads_rwlock_LDADD = libpo6.la

test_threads_thread_SOURCES = test/threads/thread.cc $(th_sources)
test_threads_thread_LDADD = libpo6.la

test_time_SOURCES = test/time.cc $(th_sources)
test_time_LDADD = libpo6.la
