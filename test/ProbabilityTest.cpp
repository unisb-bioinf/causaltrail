#include "gtest/gtest.h"
#include "../ProbabilityHandler.h"
#include "../NetworkController.h"

class ProbabilityTest : public ::testing::Test{
	protected:
	ProbabilityTest()
		:c(NetworkController())
	{
	}
	
	void virtual SetUp(){
		c.loadNetwork("TestA.na");
		c.loadNetwork("TestSif.sif");
		c.loadObservations("data.txt","control.txt");
		c.trainNetwork();
	}

	public:
	NetworkController c;
};

TEST_F(ProbabilityTest, TotalProbability){
	Network n = c.getNetwork();
	ProbabilityHandler p (n);
	//Tests for Grade
	ASSERT_NEAR(0.362f,p.computeTotalProbability(2,0),0.001);
	ASSERT_NEAR(0.2884f,p.computeTotalProbability(2,1),0.001);
	ASSERT_NEAR(0.3496f,p.computeTotalProbability(2,2),0.001);

	//Tests for Intelligence
	ASSERT_NEAR(0.7f,p.computeTotalProbability(3,0),0.001);
	ASSERT_NEAR(0.3f,p.computeTotalProbability(3,1),0.001);	

	//Tests for Difficulty
	ASSERT_NEAR(0.6f,p.computeTotalProbability(1,0),0.001);
	ASSERT_NEAR(0.4f,p.computeTotalProbability(1,1),0.001);	

	//Tests for SAT
	ASSERT_NEAR(0.725f,p.computeTotalProbability(5,0),0.001);
	ASSERT_NEAR(0.275f,p.computeTotalProbability(5,1),0.001);	

	//Tests for Letter
	ASSERT_NEAR(0.497664f,p.computeTotalProbability(4,0),0.001);
	ASSERT_NEAR(0.502336f,p.computeTotalProbability(4,1),0.001);	
}

TEST_F(ProbabilityTest, JointProbability){
	Network n = c.getNetwork();
	ProbabilityHandler p (n);
	//Test for Grade and Difficulty
	std::unordered_map<unsigned int, int> m1;
	m1[2]=0;
	m1[1]=0;
	std::vector<unsigned int> v1{2,1};
	ASSERT_NEAR(0.288f, p.computeJointProbability(v1,m1), 0.001);

	//Test for Difficulty
	std::unordered_map<unsigned int, int> m2;
	m2[1]=0;
	std::vector<unsigned int> v2 {1};
	ASSERT_NEAR(0.6f, p.computeJointProbability(v2,m2), 0.001);

	//Test for fully specified distribution
	std::unordered_map<unsigned int, int> m3;
	m3[1]=0;
	m3[2]=0;
	m3[3]=0;
	m3[4]=0;
	m3[5]=0;
	std::vector<unsigned int> v3{1,2,3,4,5};
	ASSERT_NEAR(0.01197f, p.computeJointProbability(v3,m3), 0.001);

}

TEST_F(ProbabilityTest, ConditionalProbability){
	Network n = c.getNetwork();
	ProbabilityHandler p (n);
	std::unordered_map<unsigned int, int> mn;
	mn[2]=0;
	mn[3]=0;
	mn[1]=0;
	std::vector<unsigned int> v1 {2,3,1};
	std::unordered_map<unsigned int, int> md;
	md[3]=0;
	md[1]=0;
	std::vector<unsigned int> v1d {3,1};
	//Test for Grade given Intelligence and Difficulty
	ASSERT_NEAR(0.3f, p.computeConditionalProbability(v1, v1d, mn, md), 0.001);

	std::unordered_map<unsigned int, int> mn2;
	mn2[2]=0;
	mn2[1]=0;
	std::vector<unsigned int> v2 {2,1};
	std::unordered_map<unsigned int, int> md2;
	md2[1]=0;
	std::vector<unsigned int> v2d {1};
	//Test for Grade given Difficulty
	ASSERT_NEAR(0.48f, p.computeConditionalProbability(v2, v2d, mn2, md2),0.001);

	std::unordered_map<unsigned int, int> mn3;
	mn3[5]=0;
	mn3[1]=0;
	std::vector<unsigned int> v3 {1,5};
	std::unordered_map<unsigned int, int> md3;
	md3[5]=0;
	std::vector<unsigned int> v3d {5};
	//Test for Difficulty given SAT
	ASSERT_NEAR(0.6f, p.computeConditionalProbability(v3, v3d, mn3, md3),0.001);

}

TEST_F(ProbabilityTest, maxSearch){
	Network n = c.getNetwork();
	ProbabilityHandler p (n);
	//Tests for grade
	ASSERT_NEAR(0.362f,p.maxSearch({2}).first,0.001);
	ASSERT_TRUE("g1"==p.maxSearch({2}).second[0]);

	//Tests for SAT
	ASSERT_NEAR(0.725f,p.maxSearch({5}).first,0.001);
	ASSERT_TRUE("s0"==p.maxSearch({5}).second[0]);

	//Tests for Intelligence
	ASSERT_NEAR(0.7f,p.maxSearch({3}).first,0.001);
	ASSERT_TRUE("i0"==p.maxSearch({3}).second[0]);
}
