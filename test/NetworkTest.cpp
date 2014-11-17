#include "gtest/gtest.h"
#include "../Network.h"

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
	ASSERT_THROW(n_.readNetwork("TestSif.sif"),std::invalid_argument);
	n_.readNetwork("TestA.na");
	ASSERT_TRUE(n_.getNodes().size()==5);
	n_.readNetwork("TestSif.sif");
	ASSERT_THROW(n_.getNode("Test"),std::invalid_argument);
}

TEST_F(NetworkTest, readNetworkTGF){
	n_.readNetwork("test.tgf");
	ASSERT_TRUE(n_.getNodes().size()==3);
	ASSERT_TRUE(n_.getNode("Hallo").getID()==1);
	ASSERT_TRUE(n_.getNode(2).getName()=="Welt");
	ASSERT_TRUE(n_.hasNode("!!!"));
	ASSERT_TRUE(n_.getIndex("Hallo")==0);
	ASSERT_TRUE(n_.getIndex(2)==1);
}

TEST_F(NetworkTest, getParents){
	n_.readNetwork("test.tgf");
	ASSERT_TRUE(n_.getNode("Hallo").getParents().empty());
	ASSERT_TRUE(n_.getNode(2).getParents()[0]==1);
	ASSERT_TRUE(n_.getNode(3).getParents()[0]==2);	
}

TEST_F(NetworkTest, cutParents){
	n_.readNetwork("test.tgf");
	n_.cutParents(1);
	n_.cutParents("Welt");
	n_.cutParents(3);
	ASSERT_TRUE(n_.getNode("Hallo").getParents().empty());
	ASSERT_TRUE(n_.getNode(2).getParents().empty());
	ASSERT_TRUE(n_.getNode(3).getParents().empty());
}

TEST_F(NetworkTest, backup){
	n_.readNetwork("test.tgf");
	n_.createBackup();
	n_.cutParents(1);
	n_.cutParents("Welt");
	n_.cutParents(3);
	ASSERT_TRUE(n_.getNode("Hallo").getParents().empty());
	ASSERT_TRUE(n_.getNode(2).getParents().empty());
	ASSERT_TRUE(n_.getNode(3).getParents().empty());
	n_.loadBackup();
	ASSERT_TRUE(n_.getNode("Hallo").getParents().empty());
	ASSERT_TRUE(n_.getNode(2).getParents()[0]==1);
	ASSERT_TRUE(n_.getNode(3).getParents()[0]==2);	
}

TEST_F(NetworkTest, DFS){
	std::vector<unsigned int> temp;
	n_.readNetwork("test.tgf");
	n_.performDFS(3,temp);
	ASSERT_TRUE(temp.size()==3);
	ASSERT_TRUE(temp[0]==3);
	ASSERT_TRUE(temp[1]==2);
	ASSERT_TRUE(temp[2]==1);
	n_.setAllUnvisited();
	ASSERT_FALSE(n_.getNode("Hallo").isVisited());
	ASSERT_FALSE(n_.getNode("Welt").isVisited());
	ASSERT_FALSE(n_.getNode("!!!").isVisited());
}

TEST_F(NetworkTest, edgeOperations){
	n_.readNetwork("test.tgf");
	n_.addEdge(3,1);
	ASSERT_TRUE(n_.getNode(3).getParents().size()==2);
	ASSERT_TRUE(n_.getNode(3).getParents()[0]==1);
	ASSERT_TRUE(n_.getNode(3).getParents()[1]==2);
	n_.removeEdge(3,1);
	ASSERT_TRUE(n_.getNode(3).getParents().size()==1);
	ASSERT_TRUE(n_.getNode(3).getParents()[0]==2);
}
