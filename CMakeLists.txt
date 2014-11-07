cmake_minimum_required (VERSION 2.6)

project (Causal-Analysis)

add_definitions(-std=c++11)

find_package(Boost COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS}) 

add_executable(CausalTrail main.cpp Matrix.h Node.h Node.cpp Network.h Network.cpp Combinations.h ProbabilityHandler.h ProbabilityHandler.cpp EM.h EM.cpp NetworkController.h NetworkController.cpp Discretiser.h Discretiser.cpp DataDistribution.h DataDistribution.cpp Interventions.h Interventions.cpp Parser.h Parser.cpp)

target_link_libraries(CausalTrail ${Boost_LIBRARIES})
