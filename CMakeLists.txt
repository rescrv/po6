cmake_minimum_required(VERSION 2.6)

project(po6)

# Extend the CMake module search path.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# Flags are credit to Abhishek "G" Mukherjee
set(po6_wanal "")
set(po6_wanal "${po6_wanal} -pedantic")
set(po6_wanal "${po6_wanal} -std=c++98")
set(po6_wanal "${po6_wanal} -Wabi")
set(po6_wanal "${po6_wanal} -Wall")
set(po6_wanal "${po6_wanal} -Wcast-align")
set(po6_wanal "${po6_wanal} -Wcast-qual")
set(po6_wanal "${po6_wanal} -Wctor-dtor-privacy")
set(po6_wanal "${po6_wanal} -Weffc++")
set(po6_wanal "${po6_wanal} -Wextra")
set(po6_wanal "${po6_wanal} -Wfloat-equal")
set(po6_wanal "${po6_wanal} -Wformat=2")
set(po6_wanal "${po6_wanal} -Winit-self")
set(po6_wanal "${po6_wanal} -Winline")
set(po6_wanal "${po6_wanal} -Woverloaded-virtual")
set(po6_wanal "${po6_wanal} -Wshadow")
set(po6_wanal "${po6_wanal} -Wsign-promo")
set(po6_wanal "${po6_wanal} -Wstrict-overflow=5")
set(po6_wanal "${po6_wanal} -Wswitch-default")
set(po6_wanal "${po6_wanal} -Wswitch-enum")
set(po6_wanal "${po6_wanal} -Wunsafe-loop-optimizations")
set(po6_wanal "${po6_wanal} -Wwrite-strings")

set(CMAKE_CXX_FLAGS "${po6_wanal} ${CMAKE_CXX_FLAGS}")

# Set the test sources
set(po6_test_srcs
    test/error.cc
    test/io/fd.cc
    test/net/ipaddr.cc
    test/net/location.cc
    test/net/socket.cc
    test/pathname.cc
    test/threads/cond.cc
    test/threads/mutex.cc
    test/threads/rwlock.cc
    test/threads/thread.cc
)

# Installed headers
set(po6_headers
    include/po6/error.h
    include/po6/pathname.h
)

set(po6_io_headers
    include/po6/io/fd.h
)

set(po6_net_headers
    include/po6/net/ipaddr.h
    include/po6/net/location.h
    include/po6/net/socket.h
)

set(po6_threads_headers
    include/po6/threads/cond.h
    include/po6/threads/mutex.h
    include/po6/threads/rwlock.h
    include/po6/threads/thread.h
)

# Setup the include paths
include_directories(include)

# Tests
ENABLE_TESTING()

add_executable(testrunner test/runner.cc ${po6_test_srcs})
target_link_libraries(testrunner
    pthread
    gtest
)

add_test(ThreadTest.CtorAndDtor testrunner --gtest_filter=ThreadTest.CtorAndDtor)
add_test(ThreadTest.StartAndJoin testrunner --gtest_filter=ThreadTest.StartAndJoin)
add_test(ThreadTest.DoubleStart testrunner --gtest_filter=ThreadTest.DoubleStart)
add_test(ThreadTest.DoubleJoin testrunner --gtest_filter=ThreadTest.DoubleJoin)
add_test(ThreadTest.JoinWithoutStart testrunner --gtest_filter=ThreadTest.JoinWithoutStart)
add_test(ThreadTest.StartAndDtorThrows testrunner --gtest_filter=ThreadTest.StartAndDtorThrows)
add_test(RwlockTest.CtorAndDtor testrunner --gtest_filter=RwlockTest.CtorAndDtor)
add_test(RwlockTest.LockAndUnlock testrunner --gtest_filter=RwlockTest.LockAndUnlock)
add_test(RwlockTest.TwoThreads testrunner --gtest_filter=RwlockTest.TwoThreads)
add_test(RwlockTest.Holding testrunner --gtest_filter=RwlockTest.Holding)
add_test(MutexTest.CtorAndDtor testrunner --gtest_filter=MutexTest.CtorAndDtor)
add_test(MutexTest.LockAndUnlock testrunner --gtest_filter=MutexTest.LockAndUnlock)
add_test(MutexTest.TryLock testrunner --gtest_filter=MutexTest.TryLock)
add_test(MutexTest.LockAndDtorThrows testrunner --gtest_filter=MutexTest.LockAndDtorThrows)
add_test(MutexTest.TwoThreads testrunner --gtest_filter=MutexTest.TwoThreads)
add_test(MutexTest.Holding testrunner --gtest_filter=MutexTest.Holding)
add_test(CondTest.CtorAndDtor testrunner --gtest_filter=CondTest.CtorAndDtor)
add_test(CondTest.SimpleCond testrunner --gtest_filter=CondTest.SimpleCond)
add_test(LogicErrorTest.NormallyThrows testrunner --gtest_filter=LogicErrorTest.NormallyThrows)
add_test(LogicErrorTest.ThrowInProgressDeathTest testrunner --gtest_filter=LogicErrorTest.ThrowInProgressDeathTest)
add_test(ErrorTest.CopyingAndAssigning testrunner --gtest_filter=ErrorTest.CopyingAndAssigning)
add_test(FdTest.CtorAndDtor testrunner --gtest_filter=FdTest.CtorAndDtor)
add_test(FdTest.OpenAndExplicitClose testrunner --gtest_filter=FdTest.OpenAndExplicitClose)
add_test(FdTest.OpenAndImplicitClose testrunner --gtest_filter=FdTest.OpenAndImplicitClose)
add_test(FdTest.OpenReadWriteClose testrunner --gtest_filter=FdTest.OpenReadWriteClose)
add_test(IpaddrTest.CreateAndCompare testrunner --gtest_filter=IpaddrTest.CreateAndCompare)
add_test(IpaddrTest.Assignment testrunner --gtest_filter=IpaddrTest.Assignment)
add_test(IpaddrTest.SelfAssignment testrunner --gtest_filter=IpaddrTest.SelfAssignment)
add_test(IpaddrTest.PackV4 testrunner --gtest_filter=IpaddrTest.PackV4)
add_test(IpaddrTest.PackV6 testrunner --gtest_filter=IpaddrTest.PackV6)
add_test(IpaddrTest.OutputStream testrunner --gtest_filter=IpaddrTest.OutputStream)
add_test(IpaddrTest.InputStream testrunner --gtest_filter=IpaddrTest.InputStream)
add_test(IpaddrTest.InputStreamErrors testrunner --gtest_filter=IpaddrTest.InputStreamErrors)
add_test(LocationTest.CtorAndDtor testrunner --gtest_filter=LocationTest.CtorAndDtor)
add_test(LocationTest.Comparison testrunner --gtest_filter=LocationTest.Comparison)
add_test(LocationTest.FromSockaddrIn testrunner --gtest_filter=LocationTest.FromSockaddrIn)
add_test(LocationTest.FromSockaddrIn6 testrunner --gtest_filter=LocationTest.FromSockaddrIn6)
add_test(LocationTest.FromSockaddrInCasted testrunner --gtest_filter=LocationTest.FromSockaddrInCasted)
add_test(LocationTest.FromSockaddrIn6Casted testrunner --gtest_filter=LocationTest.FromSockaddrIn6Casted)
add_test(SocketTest.ClientAndServer testrunner --gtest_filter=SocketTest.ClientAndServer)
add_test(PathnameTest.CtorAndDtor testrunner --gtest_filter=PathnameTest.CtorAndDtor)
add_test(PathnameTest.Equality testrunner --gtest_filter=PathnameTest.Equality)
add_test(PathnameTest.Basename testrunner --gtest_filter=PathnameTest.Basename)
add_test(PathnameTest.Dirname testrunner --gtest_filter=PathnameTest.Dirname)

# Installation targets.
install(
    FILES ${po6_headers}
    DESTINATION  include/po6
    COMPONENT development
)

install(
    FILES ${po6_io_headers}
    DESTINATION  include/po6/io
    COMPONENT development
)

install(
    FILES ${po6_net_headers}
    DESTINATION  include/po6/net
    COMPONENT development
)

install(
    FILES ${po6_threads_headers}
    DESTINATION  include/po6/threads
    COMPONENT development
)
