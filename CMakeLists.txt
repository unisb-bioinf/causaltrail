cmake_minimum_required (VERSION 2.6)

project (Causal-Analysis)

add_definitions(-std=c++11)

find_package(Boost COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS}) 

add_executable(CausalTrail main.cpp Matrix.h Node.h Node.cpp Network.h Network.cpp Combinations.h EM.h EM.cpp Controller.h Controller.cpp Discretiser.h Discretiser.cpp ObservationHandler.h ObservationHandler.cpp)

target_link_libraries(CausalTrail ${Boost_LIBRARIES})
