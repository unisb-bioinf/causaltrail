cmake_minimum_required (VERSION 2.8.11)

project (Causal-Analysis)

add_definitions(-std=c++11 -Wall -pedantic)

find_package(Boost COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS}) 

add_subdirectory(core)

if(Qt5Widgets_DIR)

#set(Qt5Widgets_DIR "/local/fschmidt/QT/5.3/gcc_64/lib/cmake/Qt5Widgets")
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
