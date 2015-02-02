cmake_minimum_required (VERSION 2.6)

project (Causal-Analysis-core)

add_definitions(-std=c++11 -Wall -pedantic)

add_library(CausalTrailLib Matrix.h Node.h Node.cpp Network.h Network.cpp Combinations.h ProbabilityHandler.h ProbabilityHandler.cpp EM.h EM.cpp NetworkController.h NetworkController.cpp Discretiser.h Discretiser.cpp DataDistribution.h DataDistribution.cpp Interventions.h Interventions.cpp QueryExecuter.h QueryExecuter.cpp Parser.h Parser.cpp Factor.h Factor.cpp)
target_link_libraries(CausalTrailLib ${Boost_LIBRARIES})

add_executable(CausalTrail main.cpp)
target_link_libraries(CausalTrail CausalTrailLib ${Boost_LIBRARIES})
