cmake_minimum_required (VERSION 2.8.11)

if(POLICY CMP0054)
	cmake_policy(SET CMP0054 NEW)
endif()

project (Causal-Analysis)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(cmake/CompilerSpecific.cmake)

find_package(Boost COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(core)

add_subdirectory(gui)

IF(GTEST_SRC_DIR)

enable_testing()
add_subdirectory(test)

endif()
