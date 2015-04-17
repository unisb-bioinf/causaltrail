cmake_minimum_required (VERSION 2.8.11)

if(POLICY CMP0054)
	cmake_policy(SET CMP0054 NEW)
endif()

project (Causal-Analysis)

include(cmake/CompilerSpecific.cmake)

find_package(Boost COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(core)

if(Qt5Widgets_DIR)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
get_target_property(QtCore_location Qt5::Core LOCATION)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_subdirectory(gui)

endif()

IF(GTEST_SRC_DIR)

enable_testing()
add_subdirectory(test)

endif()
