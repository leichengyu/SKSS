CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(skss)

include(cmake/Modules/FindGlog.cmake)

set(CMAKE_BUILD_TYPE Debug)

option(USE_CLANG "build application with clang" ON)
if (USE_CLANG)
    SET(CMAKE_C_COMPILER "/usr/bin/clang")
endif (USE_CLANG)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

file(GLOB SOURCES "src/*.cpp")

ADD_EXECUTABLE(skss_service ${SOURCES})

TARGET_LINK_LIBRARIES(skss_service ${GLOG_LIBRARIES})