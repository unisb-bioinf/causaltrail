#include "gtest/gtest.h"
#include "../Node.h"

class NodeTest : public ::testing::Test{
	protected:
	NodeTest()
		:n_(Node(0,1,"TestNode"))
	{
		n_.setProbability(Matrix<float>(3,3,0.5f));
		n_.setObservations(Matrix<int>(3,3,42));
		n_.setParents({1,2});
		n_.setUniqueValues({1,2,3,4});
		n_.setUniqueValuesExcludingNA({1,2,3,4});
		n_.setValueNames({"A","B","C","D"});
		n_.setValueNamesProb({"A","B","C","D"});
		n_.setParentValueNames({"A","B","C","D"});
		n_.setObservationRow(42);
	}
	public:
	Node n_;

};

TEST_F(NodeTest,getName){
	ASSERT_TRUE(n_.getName()=="TestNode");
}

TEST_F(NodeTest,getIndex){
	ASSERT_EQ(0u, n_.getIndex());
}

TEST_F(NodeTest,getID){
	ASSERT_TRUE(1==n_.getID());
}

TEST_F(NodeTest,getProbability){
	ASSERT_TRUE(0.5f==n_.getProbability(0,0));
}

TEST_F(NodeTest,setProbability){
	n_.setProbability(1.0f,0,0);
	ASSERT_TRUE(1.0f==n_.getProbability(0,0));
}

TEST_F(NodeTest,setProbabilityToOne){
	n_.setProbabilityTo1(0);
	ASSERT_TRUE(1.0f==n_.getProbability(0,0));
	ASSERT_TRUE(1.0f==n_.getProbability(0,1));
}

TEST_F(NodeTest,getObservations){
	ASSERT_EQ(42u, n_.getObservations(0,0));
}

TEST_F(NodeTest,setObservations){	
	n_.setObservations(44,0,0);
	ASSERT_EQ(44u, n_.getObservations(0,0));
}

TEST_F(NodeTest,backupDoInterventions){
	n_.createBackupDoIntervention();
	n_.setProbabilityTo1(0);
	ASSERT_TRUE(1.0f==n_.getProbability(0,0));
	n_.loadBackupDoIntervention();
	ASSERT_TRUE(0.5f==n_.getProbability(0,0));
	
}

TEST_F(NodeTest, parents){
	n_.createBackupDoIntervention();
	std::vector<unsigned int> p {1,2};
	ASSERT_TRUE(n_.getParents()==p);
	n_.cutParents();
	ASSERT_TRUE(n_.getParents().empty());
	n_.loadBackupDoIntervention();
	ASSERT_TRUE(n_.getParents()==p);
}

TEST_F(NodeTest, observationRow){
	ASSERT_EQ(42,n_.getObservationRow());
}

TEST_F(NodeTest, uniqueValues){
	std::vector<int> uniqueValues {1,2,3,4};
	ASSERT_TRUE(n_.getUniqueValues()==uniqueValues);
	ASSERT_TRUE(n_.getUniqueValuesExcludingNA()==uniqueValues);
}

TEST_F(NodeTest, valueNames){
	std::vector<std::string> valueNames {"A","B","C","D"};
	ASSERT_TRUE(n_.getValueNames()==valueNames);
	n_.addValueName("E");
	valueNames.push_back("E");
	ASSERT_TRUE(n_.getValueNames()==valueNames);
}

TEST_F(NodeTest, valueNamesProb){
	std::vector<std::string> valueNamesProb {"A","B","C","D"};
	ASSERT_TRUE(n_.getValueNamesProb()==valueNamesProb);
	n_.addValueNameProb("E");
	valueNamesProb.push_back("E");
	ASSERT_TRUE(n_.getValueNamesProb()==valueNamesProb);
}

TEST_F(NodeTest, clearNames){
	n_.clearNameVectors();
	ASSERT_TRUE(n_.getValueNames().empty());
	ASSERT_TRUE(n_.getValueNamesProb().empty());
}

TEST_F(NodeTest, visit){
	ASSERT_FALSE(n_.isVisited());
	n_.visit();
	ASSERT_TRUE(n_.isVisited());
}

TEST_F(NodeTest, reset){
	n_.reset();
	ASSERT_EQ(0u, n_.getProbabilityMatrix().getRowCount());
	ASSERT_EQ(0u, n_.getProbabilityMatrix().getColCount());
	ASSERT_EQ(0u, n_.getObservationMatrix().getRowCount());
	ASSERT_EQ(0u, n_.getObservationMatrix().getColCount());
}
