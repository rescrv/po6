if HAVE_GTEST
AM_CPPFLAGS = -I$(abs_top_srcdir)/include $(GTEST_CPPFLAGS)
AM_LDFLAGS = $(GTEST_LDFLAGS)

TESTS = $(check_PROGRAMS)
check_PROGRAMS = error io_fd net_ipaddr net_location net_socket pathname \
				 threads_cond threads_mutex threads_rwlock threads_spinlock \
				 threads_thread

error_SOURCES = runner.cc error.cc
error_LDADD = $(GTEST_LIBS)
io_fd_SOURCES = runner.cc io/fd.cc
io_fd_LDADD = $(GTEST_LIBS)
net_ipaddr_SOURCES = runner.cc net/ipaddr.cc
net_ipaddr_LDADD = $(GTEST_LIBS)
net_location_SOURCES = runner.cc net/location.cc
net_location_LDADD = $(GTEST_LIBS)
net_socket_SOURCES = runner.cc net/socket.cc
net_socket_LDADD = $(GTEST_LIBS)
pathname_SOURCES = runner.cc pathname.cc
pathname_LDADD = $(GTEST_LIBS)
threads_cond_SOURCES = runner.cc threads/cond.cc
threads_cond_LDADD = $(GTEST_LIBS)
threads_mutex_SOURCES = runner.cc threads/mutex.cc
threads_mutex_LDADD = $(GTEST_LIBS)
threads_rwlock_SOURCES = runner.cc threads/rwlock.cc
threads_rwlock_LDADD = $(GTEST_LIBS)
threads_spinlock_SOURCES = runner.cc threads/spinlock.cc
threads_spinlock_LDADD = $(GTEST_LIBS)
threads_thread_SOURCES = runner.cc threads/thread.cc
threads_thread_LDADD = $(GTEST_LIBS)
endif
