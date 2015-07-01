ACLOCAL_AMFLAGS = -I m4

AM_MAKEFLAGS = --no-print-directory

EXTRA_DIST = LICENSE README

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libpo6.pc

nobase_include_HEADERS =
nobase_include_HEADERS += po6/errno.h
nobase_include_HEADERS += po6/error.h
nobase_include_HEADERS += po6/io/fd.h
nobase_include_HEADERS += po6/io/mmap.h
nobase_include_HEADERS += po6/net/hostname.h
nobase_include_HEADERS += po6/net/ipaddr.h
nobase_include_HEADERS += po6/net/location.h
nobase_include_HEADERS += po6/net/socket.h
nobase_include_HEADERS += po6/noncopyable.h
nobase_include_HEADERS += po6/pathname.h
nobase_include_HEADERS += po6/threads/barrier.h
nobase_include_HEADERS += po6/threads/cond.h
nobase_include_HEADERS += po6/threads/mutex.h
nobase_include_HEADERS += po6/threads/rwlock.h
nobase_include_HEADERS += po6/threads/thread.h

#################################### Source ####################################

lib_LTLIBRARIES = libpo6.la
libpo6_la_SOURCES =
libpo6_la_SOURCES += errno.cc

##################################### Tests ####################################

th_sources = test/runner.cc th.cc th.h

TESTS = $(check_PROGRAMS)
check_PROGRAMS =
check_PROGRAMS += test/error
check_PROGRAMS += test/io_fd
check_PROGRAMS += test/net/hostname
check_PROGRAMS += test/net/ipaddr
check_PROGRAMS += test/net/location
check_PROGRAMS += test/net/socket
check_PROGRAMS += test/pathname
check_PROGRAMS += test/threads/cond
check_PROGRAMS += test/threads/mutex
check_PROGRAMS += test/threads/rwlock
check_PROGRAMS += test/threads/thread

test_error_SOURCES = test/error.cc $(th_sources)
test_io_fd_SOURCES = test/io/fd.cc $(th_sources)
test_net_hostname_SOURCES = test/net/hostname.cc $(th_sources)
test_net_ipaddr_SOURCES = test/net/ipaddr.cc $(th_sources)
test_net_location_SOURCES = test/net/location.cc $(th_sources)
test_net_socket_SOURCES = test/net/socket.cc $(th_sources)
test_pathname_SOURCES = test/pathname.cc $(th_sources)
test_threads_cond_SOURCES = test/threads/cond.cc $(th_sources)
test_threads_cond_LDADD = -lpthread
test_threads_mutex_SOURCES = test/threads/mutex.cc $(th_sources)
test_threads_mutex_LDADD = -lpthread
test_threads_rwlock_SOURCES = test/threads/rwlock.cc $(th_sources)
test_threads_rwlock_LDADD = -lpthread
test_threads_thread_SOURCES = test/threads/thread.cc $(th_sources)
test_threads_thread_LDADD = -lpthread
