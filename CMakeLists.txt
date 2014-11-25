cmake_minimum_required (VERSION 2.6)

project (Causal-Analysis)

add_definitions(-std=c++11 -Wall -pedantic)

find_package(Boost COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS}) 

IF(GTEST_SRC_DIR)

enable_testing()
add_subdirectory(${GTEST_SRC_DIR} "gtest")

add_executable(CausalTrail main.cpp Matrix.h Node.h Node.cpp Network.h Network.cpp Combinations.h ProbabilityHandler.h ProbabilityHandler.cpp EM.h EM.cpp NetworkController.h NetworkController.cpp Discretiser.h Discretiser.cpp DataDistribution.h DataDistribution.cpp Interventions.h Interventions.cpp QueryExecuter.h QueryExecuter.cpp Parser.h Parser.cpp)
target_link_libraries(CausalTrail ${Boost_LIBRARIES})


add_executable(runMatrixTests test/MatrixTest.cpp)
target_link_libraries(runMatrixTests gtest gtest_main)
add_test(NAME runMatrixTests COMMAND runMatrixTests)

add_executable(runNodeTests test/NodeTest.cpp Node.cpp)
target_link_libraries(runNodeTests gtest gtest_main)
add_test(NAME runNodeTests COMMAND runNodeTests)

add_executable(runNetworkTests test/NetworkTest.cpp Network.cpp Node.cpp)
target_link_libraries(runNetworkTests ${Boost_LIBRARIES} gtest gtest_main)
add_test(NAME runNetworkTests COMMAND runNetworkTests)

add_executable(runDiscretiserTest test/DiscretiserTest.cpp Discretiser.cpp Network.cpp Node.cpp)
target_link_libraries(runDiscretiserTest ${Boost_LIBRARIES} gtest gtest_main)
add_test(NAME runDiscretiserTest COMMAND runDiscretiserTest)

add_executable(runCombinationsTests test/CombinationsTest.cpp)
target_link_libraries(runCombinationsTests gtest gtest_main)
add_test(NAME runCombinationsTests COMMAND runCombinationsTests)

add_executable(runEMTests test/EMTest.cpp EM.cpp NetworkController.cpp Node.cpp Network.cpp ProbabilityHandler.cpp DataDistribution.cpp Discretiser.cpp)
target_link_libraries(runEMTests ${Boost_LIBRARIES} gtest gtest_main)
add_test(NAME runEMTests COMMAND runEMTests)

add_executable(runInterventionTests test/InterventionTest.cpp Interventions.cpp EM.cpp NetworkController.cpp Node.cpp Network.cpp ProbabilityHandler.cpp DataDistribution.cpp Discretiser.cpp)
target_link_libraries(runInterventionTests ${Boost_LIBRARIES} gtest gtest_main)
add_test(NAME runInterventionTests COMMAND runInterventionTests)

add_executable(runProbabilityTests test/ProbabilityTest.cpp Interventions.cpp EM.cpp NetworkController.cpp Node.cpp Network.cpp ProbabilityHandler.cpp DataDistribution.cpp Discretiser.cpp)
target_link_libraries(runProbabilityTests ${Boost_LIBRARIES} gtest gtest_main)
add_test(NAME runProbabilityTests COMMAND runProbabilityTests)

add_executable(runQueryTests test/QueryTest.cpp Interventions.cpp EM.cpp NetworkController.cpp Node.cpp Network.cpp ProbabilityHandler.cpp DataDistribution.cpp Discretiser.cpp QueryExecuter.cpp Parser.cpp)
target_link_libraries(runQueryTests ${Boost_LIBRARIES} gtest gtest_main)
add_test(NAME runQueryTests COMMAND runQueryTests)

endif()
