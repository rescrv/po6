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
)

# Installed headers
set(po6_headers
    include/error.h
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

add_test(LogicErrorTest testrunner --gtest_filter=LogicErrorTest.*)

# Installation targets.
install(
    FILES ${po6_headers}
    DESTINATION  include/po6
    COMPONENT development
)
