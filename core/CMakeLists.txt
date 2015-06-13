cmake_minimum_required (VERSION 2.6)

project (Causal-Analysis-core)

add_library(CausalTrailLib Matrix.h Node.h Node.cpp Network.h Network.cpp Combinations.h DataDistribution.h DataDistribution.cpp SerializeDeserializeJson.h SerializeDeserializeJson.cpp Discretisations.h Discretisations.cpp DiscretiseFloor.h DiscretiseFloor.cpp DiscretiseArithmeticMean.h DiscretiseArithmeticMean.cpp DiscretiseMedian.h DiscretiseMedian.cpp DiscretiseRound.h DiscretiseRound.cpp DiscretiseBracketMedians.h DiscretiseBracketMedians.cpp DiscretiseHarmonicMean.h DiscretiseHarmonicMean.cpp DiscretisePT.h DiscretisePT.cpp DiscretiseThreshold.h DiscretiseThreshold.cpp DiscretiseCeil.h DiscretiseCeil.cpp DiscretiseMapping.h DiscretiseMapping.cpp DiscretiseZScore.h DiscretiseZScore.cpp DiscretisationFactory.h DiscretisationFactory.cpp Discretiser.h Discretiser.cpp ProbabilityHandler.h ProbabilityHandler.cpp EM.h EM.cpp NetworkController.h NetworkController.cpp Interventions.h Interventions.cpp QueryExecuter.h QueryExecuter.cpp Parser.h Parser.cpp Factor.h Factor.cpp) 
target_link_libraries(CausalTrailLib ${Boost_LIBRARIES})

add_executable(CausalTrail main.cpp)
target_link_libraries(CausalTrail CausalTrailLib ${Boost_LIBRARIES})
