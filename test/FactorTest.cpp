#include "gtest/gtest.h"
#include "../Factor.h"
#include "../NetworkController.h"
#include "config.h"

class FactorTest : public ::testing::Test{
	protected:
	FactorTest()
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

TEST_F(FactorTest, FactorConstructorTestNode){
	Network n = c.getNetwork();
	std::vector<int> emptyValues (5,-1);
	Factor f (n.getNode("Grade"), emptyValues);
	SUCCEED();
}

TEST_F(FactorTest, FactorConstructorTestDirect){
	std::vector<unsigned int> testIds = {0,1,2,3};
	Factor f (100,testIds);
	SUCCEED();
}

TEST_F(FactorTest, getIDs){
	std::vector<unsigned int> testIds = {0,1,2,3};
	Factor f (10,testIds);
	ASSERT_TRUE(testIds == f.getIDs());
}

TEST_F(FactorTest, knownValuesGetProbability){
	Network n = c.getNetwork();
	std::vector<int> values (5,-1);
	values[2]=1;
	Factor f (n.getNode("Grade"), values);
	ASSERT_NEAR(0.9f, f.getProbability(0),0.001);
	ASSERT_NEAR(0.5f, f.getProbability(1),0.001);
	ASSERT_NEAR(0.08f, f.getProbability(2),0.001);
	ASSERT_NEAR(0.3f, f.getProbability(3),0.001);
	ASSERT_NEAR(0.02f, f.getProbability(4),0.001);
	ASSERT_NEAR(0.2f, f.getProbability(5),0.001);
	
}

TEST_F(FactorTest, addGetProbability){
	std::vector<unsigned int> testIds = {0,1,2,3};
	Factor f (10,testIds);
	f.addProbability(0.612f);
	ASSERT_TRUE(0.612f == f.getProbability(10));
}

TEST_F(FactorTest, setGetProbability){
	std::vector<unsigned int> testIds = {0,1,2,3};
	Factor f (10,testIds);
	f.setProbability(0.429f, 1);
	ASSERT_TRUE(0.429f == f.getProbability(1));
}

TEST_F(FactorTest, getIndex){	
	std::vector<unsigned int> testIds = {3,2,1,0};
	Factor f (10,testIds);
	ASSERT_EQ(0u, f.getIndex(3));
	ASSERT_EQ(1u, f.getIndex(2));
	ASSERT_EQ(2u, f.getIndex(1));
	ASSERT_EQ(3u, f.getIndex(0));
	ASSERT_THROW(f.getIndex(4), std::invalid_argument);	
}

TEST_F(FactorTest, getProbabilityVector){
	Network n = c.getNetwork();
	std::vector<int> emptyValues (5,-1);
	Factor fGrade (n.getNode("Grade"), emptyValues);
	ASSERT_NEAR(1.0f,fGrade.getProbability(emptyValues),0.001);
	Factor fIntelligence(n.getNode("Intelligence"), emptyValues);
	emptyValues[2]=0;
	ASSERT_NEAR(0.7f,fIntelligence.getProbability(emptyValues), 0.001);

}

TEST_F(FactorTest, product){
	Network n = c.getNetwork();
	std::vector<int> emptyValues (5,-1);
	Factor fGrade (n.getNode("Grade"), emptyValues);
	Factor fIntelligence (n.getNode("Intelligence"), emptyValues);
	Factor product = fGrade.product(fIntelligence, n, emptyValues);
	ASSERT_TRUE(fGrade.getIDs() == product.getIDs());
	ASSERT_NEAR(0.21f,product.getProbability(0),0.001);
	ASSERT_NEAR(0.27f,product.getProbability(1),0.001);
	ASSERT_NEAR(0.035f,product.getProbability(2),0.001);
	ASSERT_NEAR(0.15f,product.getProbability(3),0.001);
	ASSERT_NEAR(0.28f,product.getProbability(4),0.001);
	ASSERT_NEAR(0.024f,product.getProbability(5),0.001);
	ASSERT_NEAR(0.175f,product.getProbability(6),0.001);
	ASSERT_NEAR(0.09f,product.getProbability(7),0.001);
	ASSERT_NEAR(0.21f,product.getProbability(8),0.001);
	ASSERT_NEAR(0.006f,product.getProbability(9),0.001);
	ASSERT_NEAR(0.49f,product.getProbability(10),0.001);
	ASSERT_NEAR(0.06f,product.getProbability(11),0.001);
}

TEST_F(FactorTest, sumOut){
	Network n = c.getNetwork();
	std::vector<int> emptyValues (5,-1);
	Factor fGrade (n.getNode("Grade"), emptyValues);
	Factor fIntelligence (n.getNode("Intelligence"), emptyValues);
	Factor product = fGrade.product(fIntelligence, n, emptyValues);
	Factor sumOut = product.sumOut(n.getNode("Intelligence").getID(), n, emptyValues);
	std::vector<unsigned int> newIDs {1,0};
	ASSERT_TRUE(newIDs == sumOut.getIDs());
	ASSERT_NEAR(0.48f,sumOut.getProbability(0),0.001);
	ASSERT_NEAR(0.185f,sumOut.getProbability(1),0.001);
	ASSERT_NEAR(0.304f,sumOut.getProbability(2),0.001);
	ASSERT_NEAR(0.265f,sumOut.getProbability(3),0.001);
	ASSERT_NEAR(0.216f,sumOut.getProbability(4),0.001);
	ASSERT_NEAR(0.55f,sumOut.getProbability(5),0.001);
}


TEST_F(FactorTest, normalize){
	std::vector<unsigned int> testIds = {0};
	Factor f (3,testIds);
	f.setProbability(0.612,0);
	f.setProbability(0.411,1);
	f.setProbability(0.429,2);
	f.normalize();
	ASSERT_NEAR(0.421488f, f.getProbability(0),0.001);
	ASSERT_NEAR(0.283058f, f.getProbability(1),0.001);
	ASSERT_NEAR(0.295455f, f.getProbability(2),0.001);
}

	
