cmake_minimum_required(VERSION 3.1)

if(POLICY CMP0054)
	cmake_policy(SET CMP0054 NEW)
endif()

project(CausalAnalysis CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(cmake/CompilerSpecific.cmake)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(core)

add_subdirectory(gui)

IF(GTEST_SRC_DIR)

enable_testing()
add_subdirectory(test)

endif()
