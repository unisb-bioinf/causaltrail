#include "gtest/gtest.h"
#include "../Network.h"
#include "config.h"

class NetworkTest : public ::testing::Test{
	protected:
	NetworkTest()
		:n_(Network())
	{
	}
	public:
	Network n_;

};

TEST_F(NetworkTest, readNetworkNASIF){
	ASSERT_THROW(n_.readNetwork(TEST_DATA_PATH("Student.sif")),std::invalid_argument);
	n_.readNetwork(TEST_DATA_PATH("Student.na"));
	ASSERT_TRUE(n_.getNodes().size()==5);
	n_.readNetwork(TEST_DATA_PATH("Student.sif"));
	ASSERT_THROW(n_.getNode("Test"),std::invalid_argument);
}

TEST_F(NetworkTest, readNetworkTGF){
	n_.readNetwork(TEST_DATA_PATH("test.tgf"));
	ASSERT_TRUE(n_.getNodes().size()==3);
	ASSERT_TRUE(n_.getNode("Hallo").getID()==0);
	ASSERT_TRUE(n_.getNode(1).getName()=="Welt");
	ASSERT_TRUE(n_.hasNode("!!!"));
	ASSERT_TRUE(n_.getIndex("Hallo")==0);
	ASSERT_TRUE(n_.getIndex(2)==2);
}

TEST_F(NetworkTest, getParents){
	n_.readNetwork(TEST_DATA_PATH("test.tgf"));
	ASSERT_TRUE(n_.getNode("Hallo").getParents().empty());
	ASSERT_TRUE(n_.getNode(1).getParents()[0]==0);
	ASSERT_TRUE(n_.getNode(2).getParents()[0]==1);	
}

TEST_F(NetworkTest, cutParents){
	n_.readNetwork(TEST_DATA_PATH("test.tgf"));
	n_.cutParents(0);
	n_.cutParents("Welt");
	n_.cutParents(2);
	ASSERT_TRUE(n_.getNode("Hallo").getParents().empty());
	ASSERT_TRUE(n_.getNode(1).getParents().empty());
	ASSERT_TRUE(n_.getNode(2).getParents().empty());
}

TEST_F(NetworkTest, backup){
	n_.readNetwork(TEST_DATA_PATH("test.tgf"));
	n_.createBackup();
	n_.cutParents(0);
	n_.cutParents("Welt");
	n_.cutParents(2);
	ASSERT_TRUE(n_.getNode("Hallo").getParents().empty());
	ASSERT_TRUE(n_.getNode(1).getParents().empty());
	ASSERT_TRUE(n_.getNode(2).getParents().empty());
	n_.loadBackup();
	ASSERT_TRUE(n_.getNode("Hallo").getParents().empty());
	ASSERT_TRUE(n_.getNode(1).getParents()[0]==0);
	ASSERT_TRUE(n_.getNode(2).getParents()[0]==1);	
}

TEST_F(NetworkTest, DFS){
	std::vector<unsigned int> temp;
	n_.readNetwork(TEST_DATA_PATH("test.tgf"));
	n_.performDFS(2,temp);
	ASSERT_TRUE(temp.size()==3);
	ASSERT_TRUE(temp[0]==2);
	ASSERT_TRUE(temp[1]==1);
	ASSERT_TRUE(temp[2]==0);
	n_.setAllUnvisited();
	ASSERT_FALSE(n_.getNode("Hallo").isVisited());
	ASSERT_FALSE(n_.getNode("Welt").isVisited());
	ASSERT_FALSE(n_.getNode("!!!").isVisited());
}

TEST_F(NetworkTest, edgeOperations){
	n_.readNetwork(TEST_DATA_PATH("test.tgf"));
	n_.addEdge(2,0);
	ASSERT_TRUE(n_.getNode(2).getParents().size()==2);
	ASSERT_TRUE(n_.getNode(2).getParents()[0]==0);
	ASSERT_TRUE(n_.getNode(2).getParents()[1]==1);
	n_.removeEdge(2,0);
	ASSERT_TRUE(n_.getNode(2).getParents().size()==1);
	ASSERT_TRUE(n_.getNode(2).getParents()[0]==1);
}
