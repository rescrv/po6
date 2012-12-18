ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS =
# -fpermissive needed for gtest
AM_CXXFLAGS = $(WANAL_CXXFLAGS) -fpermissive
AM_LDFLAGS =

EXTRA_DIST = LICENSE README

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libpo6.pc

nobase_include_HEADERS = po6/io/fd.h po6/error.h po6/noncopyable.h \
						 po6/threads/rwlock.h po6/threads/cond.h \
						 po6/threads/spinlock.h po6/threads/thread.h \
						 po6/threads/mutex.h po6/threads/barrier.h \
						 po6/pathname.h po6/net/hostname.h po6/net/ipaddr.h \
						 po6/net/location.h po6/net/socket.h

##################################### Tests ####################################

if HAVE_GTEST
AM_CPPFLAGS += -I$(abs_top_srcdir) $(GTEST_CPPFLAGS)
AM_LDFLAGS += $(GTEST_LDFLAGS)

TESTS = $(check_PROGRAMS)
check_PROGRAMS = \
			test/error \
			test/io_fd \
			test/net_hostname \
			test/net_ipaddr \
			test/net_location \
			test/net_socket \
			test/pathname \
			test/threads_cond \
			test/threads_mutex \
			test/threads_rwlock \
			test/threads_spinlock \
			test/threads_thread

test_error_SOURCES = test/runner.cc test/error.cc
test_error_LDADD = $(GTEST_LIBS)
test_io_fd_SOURCES = test/runner.cc test/io/fd.cc
test_io_fd_LDADD = $(GTEST_LIBS)
test_net_hostname_SOURCES = test/runner.cc test/net/hostname.cc
test_net_hostname_LDADD = $(GTEST_LIBS)
test_net_ipaddr_SOURCES = test/runner.cc test/net/ipaddr.cc
test_net_ipaddr_LDADD = $(GTEST_LIBS)
test_net_location_SOURCES = test/runner.cc test/net/location.cc
test_net_location_LDADD = $(GTEST_LIBS)
test_net_socket_SOURCES = test/runner.cc test/net/socket.cc
test_net_socket_LDADD = $(GTEST_LIBS)
test_pathname_SOURCES = test/runner.cc test/pathname.cc
test_pathname_LDADD = $(GTEST_LIBS)
test_threads_cond_SOURCES = test/runner.cc test/threads/cond.cc
test_threads_cond_LDADD = $(GTEST_LIBS) -lpthread
test_threads_mutex_SOURCES = test/runner.cc test/threads/mutex.cc
test_threads_mutex_LDADD = $(GTEST_LIBS) -lpthread
test_threads_rwlock_SOURCES = test/runner.cc test/threads/rwlock.cc
test_threads_rwlock_LDADD = $(GTEST_LIBS) -lpthread
test_threads_spinlock_SOURCES = test/runner.cc test/threads/spinlock.cc
test_threads_spinlock_LDADD = $(GTEST_LIBS) -lpthread
test_threads_thread_SOURCES = test/runner.cc test/threads/thread.cc
test_threads_thread_LDADD = $(GTEST_LIBS) -lpthread
endif
