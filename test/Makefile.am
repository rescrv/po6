check_PROGRAMS = testrunner
TESTS = $(check_PROGRAMS)

testrunner_SOURCES = runner.cc error.cc io/fd.cc net/ipaddr.cc net/location.cc \
					 net/socket.cc pathname.cc threads/cond.cc \
					 threads/mutex.cc threads/rwlock.cc \
					 threads/spinlock.cc threads/thread.cc
testrunner_CPPFLAGS = -I../include
testrunner_LDFLAGS = -lpthread -lgtest
