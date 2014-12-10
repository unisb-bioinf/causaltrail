cmake_minimum_required (VERSION 2.6)

project (Causal-Analysis)

add_definitions(-std=c++11 -Wall -pedantic)

find_package(Boost COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS}) 

add_library(CausalTrailLib Matrix.h Node.h Node.cpp Network.h Network.cpp Combinations.h ProbabilityHandler.h ProbabilityHandler.cpp EM.h EM.cpp NetworkController.h NetworkController.cpp Discretiser.h Discretiser.cpp DataDistribution.h DataDistribution.cpp Interventions.h Interventions.cpp QueryExecuter.h QueryExecuter.cpp Parser.h Parser.cpp Factor.h Factor.cpp)
target_link_libraries(CausalTrailLib ${Boost_LIBRARIES})

add_executable(CausalTrail main.cpp)
target_link_libraries(CausalTrail CausalTrailLib ${Boost_LIBRARIES})

IF(GTEST_SRC_DIR)

enable_testing()
include(CausalAnalysisTest.cmake)
create_test_config_file()
add_subdirectory(${GTEST_SRC_DIR} "gtest")
include_directories("${GTEST_SRC_DIR}/include")

add_executable(runMatrixTests test/MatrixTest.cpp)
target_link_libraries(runMatrixTests gtest gtest_main)
add_test(NAME runMatrixTests COMMAND runMatrixTests)

add_executable(runNodeTests test/NodeTest.cpp)
target_link_libraries(runNodeTests CausalTrailLib gtest gtest_main)
add_test(NAME runNodeTests COMMAND runNodeTests)

add_executable(runNetworkTests test/NetworkTest.cpp)
target_link_libraries(runNetworkTests CausalTrailLib ${Boost_LIBRARIES} gtest gtest_main)
add_test(NAME runNetworkTests COMMAND runNetworkTests)

add_executable(runDiscretiserTest test/DiscretiserTest.cpp)
target_link_libraries(runDiscretiserTest CausalTrailLib ${Boost_LIBRARIES} gtest gtest_main)
add_test(NAME runDiscretiserTest COMMAND runDiscretiserTest)

add_executable(runCombinationsTests test/CombinationsTest.cpp)
target_link_libraries(runCombinationsTests CausalTrailLib gtest gtest_main)
add_test(NAME runCombinationsTests COMMAND runCombinationsTests)

add_executable(runEMTests test/EMTest.cpp)
target_link_libraries(runEMTests ${Boost_LIBRARIES} CausalTrailLib gtest gtest_main)
add_test(NAME runEMTests COMMAND runEMTests)

add_executable(runInterventionTests test/InterventionTest.cpp)
target_link_libraries(runInterventionTests ${Boost_LIBRARIES} CausalTrailLib gtest gtest_main)
add_test(NAME runInterventionTests COMMAND runInterventionTests)

add_executable(runProbabilityTests test/ProbabilityTest.cpp)
target_link_libraries(runProbabilityTests ${Boost_LIBRARIES} CausalTrailLib gtest gtest_main)
add_test(NAME runProbabilityTests COMMAND runProbabilityTests)

add_executable(runQueryTests test/QueryTest.cpp)
target_link_libraries(runQueryTests ${Boost_LIBRARIES} CausalTrailLib gtest gtest_main)
add_test(NAME runQueryTests COMMAND runQueryTests)

add_executable(runFactorTests test/FactorTest.cpp)
target_link_libraries(runFactorTests ${Boost_LIBRARIES} CausalTrailLib gtest gtest_main)
add_test(NAME runFactorTests COMMAND runFactorTests)


endif()
