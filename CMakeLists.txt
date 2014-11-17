cmake_minimum_required (VERSION 2.6)

project (Causal-Analysis)

add_definitions(-std=c++11)

find_package(Boost COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS}) 

enable_testing()
add_subdirectory(/home/student/fschmidt/source/gtest-1.7.0 home/student/fschmidt/source/gtest-1.7.0/build)
include_directories(${gtest_SOURCE_DIR}/include)

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


