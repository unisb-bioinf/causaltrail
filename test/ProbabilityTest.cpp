#include "gtest/gtest.h"
#include "../core/ProbabilityHandler.h"
#include "../core/NetworkController.h"
#include "config.h"

class ProbabilityTest : public ::testing::Test{
	protected:
	ProbabilityTest()
		:c(NetworkController())
	{
	}
	
	void virtual SetUp(){
		c.loadNetwork(TEST_DATA_PATH("Student.na"));
		c.loadNetwork(TEST_DATA_PATH("Student.sif"));
		c.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("controlStudent.txt"));
		c.trainNetwork();
	}

	public:
	NetworkController c;
};

TEST_F(ProbabilityTest, Constructor){
	Network n = c.getNetwork();
	ProbabilityHandler p (n);
	SUCCEED();
}

TEST_F(ProbabilityTest, TotalProbability){
	Network n = c.getNetwork();
	ProbabilityHandler p (n);
	//Tests for Grade
	ASSERT_NEAR(0.362f,p.computeTotalProbability(1,0),0.001);
	ASSERT_NEAR(0.2884f,p.computeTotalProbability(1,1),0.001);
	ASSERT_NEAR(0.3496f,p.computeTotalProbability(1,2),0.001);

	//Tests for Intelligence
	ASSERT_NEAR(0.7f,p.computeTotalProbability(2,0),0.001);
	ASSERT_NEAR(0.3f,p.computeTotalProbability(2,1),0.001);	

	//Tests for Difficulty
	ASSERT_NEAR(0.6f,p.computeTotalProbability(0,0),0.001);
	ASSERT_NEAR(0.4f,p.computeTotalProbability(0,1),0.001);	

	//Tests for SAT
	ASSERT_NEAR(0.725f,p.computeTotalProbability(3,0),0.001);
	ASSERT_NEAR(0.275f,p.computeTotalProbability(3,1),0.001);	

	//Tests for Letter
	ASSERT_NEAR(0.497664f,p.computeTotalProbability(4,0),0.001);
	ASSERT_NEAR(0.502336f,p.computeTotalProbability(4,1),0.001);	
}

TEST_F(ProbabilityTest, TotalProbabilityNormalized){
	Network n = c.getNetwork();
	ProbabilityHandler p (n);
	//Tests for Grade
	ASSERT_NEAR(0.362f,p.computeTotalProbabilityNormalized(1,0),0.001);
	ASSERT_NEAR(0.2884f,p.computeTotalProbabilityNormalized(1,1),0.001);
	ASSERT_NEAR(0.3496f,p.computeTotalProbabilityNormalized(1,2),0.001);

	//Tests for Intelligence
	ASSERT_NEAR(0.7f,p.computeTotalProbabilityNormalized(2,0),0.001);
	ASSERT_NEAR(0.3f,p.computeTotalProbabilityNormalized(2,1),0.001);	

	//Tests for Difficulty
	ASSERT_NEAR(0.6f,p.computeTotalProbabilityNormalized(0,0),0.001);
	ASSERT_NEAR(0.4f,p.computeTotalProbabilityNormalized(0,1),0.001);	

	//Tests for SAT
	ASSERT_NEAR(0.725f,p.computeTotalProbabilityNormalized(3,0),0.001);
	ASSERT_NEAR(0.275f,p.computeTotalProbabilityNormalized(3,1),0.001);	

	//Tests for Letter
	ASSERT_NEAR(0.497664f,p.computeTotalProbabilityNormalized(4,0),0.001);
	ASSERT_NEAR(0.502336f,p.computeTotalProbabilityNormalized(4,1),0.001);	
}
	

TEST_F(ProbabilityTest, JointProbability){
	Network n = c.getNetwork();
	ProbabilityHandler p (n);
	//Test for Grade and Difficulty
	std::vector<int> m1(5,-1);
	m1[0]=0;
	m1[1]=0;
	std::vector<unsigned int> v1{0,1};
	ASSERT_NEAR(0.288f, p.computeJointProbabilityUsingVariableElimination(v1,m1),0.001);

	//Test for Difficulty
	std::vector<int> m2(5,-1);
	m2[0]=0;
	std::vector<unsigned int> v2 {0};
	ASSERT_NEAR(0.6f, p.computeJointProbabilityUsingVariableElimination(v2,m2),0.001);


	//Test for Difficulty and Intelligence
	std::vector<int> m4(5,-1);
	m4[0]=0;
	m4[2]=0;
	std::vector<unsigned int> v4 {0,2};
	ASSERT_NEAR(0.42f, p.computeJointProbabilityUsingVariableElimination(v4,m4), 0.001);

	//Test for fully specified distribution
	std::vector<int> m3(5,0);
	std::vector<unsigned int> v3{0,1,2,3,4};
	ASSERT_NEAR(0.01197f, p.computeJointProbabilityUsingVariableElimination(v3,m3),0.001);


}

TEST_F(ProbabilityTest, ConditionalProbability){
	Network n = c.getNetwork();
	ProbabilityHandler p (n);
	std::vector<int> mn(5,-1);
	mn[1]=0;
	
	std::vector<unsigned int> v1 {1};
	std::vector<int> md(5,-1);
	md[2]=0;
	md[0]=0;
	std::vector<unsigned int> v1d {0,2};
	//Test for Grade given Intelligence and Difficulty
	ASSERT_NEAR(0.3f, p.computeConditionalProbability(v1, v1d, mn, md), 0.001);

	std::vector<int> mn2(5,-1);
	mn2[1]=0;
	std::vector<unsigned int> v2 {1};
	std::vector<int> md2(5,-1);
	md2[0]=0;
	std::vector<unsigned int> v2d {0};
	//Test for Grade given Difficulty
	ASSERT_NEAR(0.48f, p.computeConditionalProbability(v2, v2d, mn2, md2),0.001);

	std::vector<int> mn3(5,-1);
	mn3[0]=0;
	std::vector<unsigned int> v3 {0};
	std::vector<int> md3(5,-1);
	md3[3]=0;
	std::vector<unsigned int> v3d {3};
	//Test for Difficulty given SAT
	ASSERT_NEAR(0.6f, p.computeConditionalProbability(v3, v3d, mn3, md3),0.001);

	std::vector<int> mn4(5,-1);
	mn4[3]=0;
	std::vector<unsigned int> v4 {3};
	std::vector<int> md4(5,-1);
	md4[0]=0;
	std::vector<unsigned int> v4d {0};
	//Test for Difficulty given Grade
	ASSERT_NEAR(0.725f, p.computeConditionalProbability(v4, v4d, mn4, md4),0.001);
}


TEST_F(ProbabilityTest, maxSearch){
	Network n = c.getNetwork();
	ProbabilityHandler p (n);
	std::vector<unsigned int> emptyNodeIDs {};
	std::vector<int> emptyNodeValues (5,-1);
	//Tests for grade
	std::pair<float,std::vector<std::string>> result1 = p.maxSearch({1},emptyNodeIDs, emptyNodeValues);
	ASSERT_NEAR(0.362f,result1.first,0.001);
	ASSERT_TRUE("g1"==result1.second[0]);

	//Tests for SAT
	std::pair<float,std::vector<std::string>> result2 = p.maxSearch({3}, emptyNodeIDs, emptyNodeValues); 
	ASSERT_NEAR(0.725f,result2.first,0.001);
	ASSERT_TRUE("s0"==result2.second[0]);

	//Tests for Intelligence
	std::pair<float,std::vector<std::string>> result3 = p.maxSearch({2}, emptyNodeIDs, emptyNodeValues);
	ASSERT_NEAR(0.7f,result3.first,0.001);
	ASSERT_TRUE("i0"==result3.second[0]);

	//Test for Grade given Intelligence
	emptyNodeIDs.push_back(2);
	emptyNodeValues[2]=1;
	std::pair<float,std::vector<std::string>> result4 = p.maxSearch({1}, emptyNodeIDs, emptyNodeValues);
	ASSERT_NEAR(0.74f,result4.first,0.001);
	ASSERT_TRUE("g1"==result4.second[0]);
}

TEST_F(ProbabilityTest, computeLikelihodOfTheData){
	Network n = c.getNetwork();
	ProbabilityHandler p (n);
	Matrix<int> testObservations(2,5,0);
	ASSERT_NEAR(-3.7322,p.calculateLikelihoodOfTheData(testObservations),0.001);

	Matrix<int> testObservations2(0,0,0);
	ASSERT_THROW(p.calculateLikelihoodOfTheData(testObservations2),std::invalid_argument);

}
