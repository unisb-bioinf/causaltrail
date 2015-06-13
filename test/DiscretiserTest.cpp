#include "gtest/gtest.h"
#include "../core/Discretiser.h"
#include "config.h"

class DiscretiserTest : public ::testing::Test{
	protected:
	DiscretiserTest()
	{

	}
	public:

};

TEST_F(DiscretiserTest,Floor){
	Matrix<std::string> oriObs (TEST_DATA_PATH("testObservations.txt"),false,true);
	Matrix<int> dObs (oriObs.getColCount(), oriObs.getRowCount(),0);
	Network n;
	Discretiser d (oriObs,TEST_DATA_PATH("jsonDiscretiserTest.json"),dObs,n);
	ASSERT_EQ(0,dObs(0,0));
	ASSERT_EQ(1,dObs(1,0));
	ASSERT_EQ(1,dObs(2,0));
	ASSERT_EQ(2,dObs(3,0));
	ASSERT_EQ(3,dObs(4,0));
	ASSERT_EQ(4,dObs(5,0));
}

TEST_F(DiscretiserTest,Ceil){
	Matrix<std::string> oriObs (TEST_DATA_PATH("testObservations.txt"),false,true);
	Matrix<int> dObs (oriObs.getColCount(), oriObs.getRowCount(),0);
	Network n;
	Discretiser d (oriObs,TEST_DATA_PATH("jsonDiscretiserTest.json"),dObs,n);
	ASSERT_EQ(0,dObs(0,1));
	ASSERT_EQ(1,dObs(1,1));
	ASSERT_EQ(1,dObs(2,1));
	ASSERT_EQ(2,dObs(3,1));
	ASSERT_EQ(3,dObs(4,1));
	ASSERT_EQ(4,dObs(5,1));
}

TEST_F(DiscretiserTest,Round){
	Matrix<std::string> oriObs (TEST_DATA_PATH("testObservations.txt"),false,true);
	Matrix<int> dObs (oriObs.getColCount(), oriObs.getRowCount(),0);
	Network n;
	Discretiser d (oriObs,TEST_DATA_PATH("jsonDiscretiserTest.json"),dObs,n);
	ASSERT_EQ(0,dObs(0,2));
	ASSERT_EQ(0,dObs(1,2));
	ASSERT_EQ(0,dObs(2,2));
	ASSERT_EQ(1,dObs(3,2));
	ASSERT_EQ(2,dObs(4,2));
	ASSERT_EQ(3,dObs(5,2));
}

TEST_F(DiscretiserTest,AMean){
	Matrix<std::string> oriObs (TEST_DATA_PATH("testObservations.txt"),false,true);
	Matrix<int> dObs (oriObs.getColCount(), oriObs.getRowCount(),0);
	Network n;
	Discretiser d (oriObs,TEST_DATA_PATH("jsonDiscretiserTest.json"),dObs,n);
	ASSERT_EQ(0,dObs(0,3));
	ASSERT_EQ(0,dObs(1,3));
	ASSERT_EQ(0,dObs(2,3));
	ASSERT_EQ(1,dObs(3,3));
	ASSERT_EQ(1,dObs(4,3));
	ASSERT_EQ(1,dObs(5,3));
}

TEST_F(DiscretiserTest,HMean){
	Matrix<std::string> oriObs (TEST_DATA_PATH("testObservations.txt"),false,true);
	Matrix<int> dObs (oriObs.getColCount(), oriObs.getRowCount(),0);
	Network n;
	Discretiser d (oriObs,TEST_DATA_PATH("jsonDiscretiserTest.json"),dObs,n);
	ASSERT_EQ(0,dObs(0,4));
	ASSERT_EQ(0,dObs(1,4));
	ASSERT_EQ(0,dObs(2,4));
	ASSERT_EQ(1,dObs(3,4));
	ASSERT_EQ(1,dObs(4,4));
	ASSERT_EQ(1,dObs(5,4));
}

TEST_F(DiscretiserTest,Median){
	Matrix<std::string> oriObs (TEST_DATA_PATH("testObservations.txt"),false,true);
	Matrix<int> dObs (oriObs.getColCount(), oriObs.getRowCount(),0);
	Network n;
	Discretiser d (oriObs,TEST_DATA_PATH("jsonDiscretiserTest.json"),dObs,n);
	ASSERT_EQ(0,dObs(0,5));
	ASSERT_EQ(0,dObs(1,5));
	ASSERT_EQ(0,dObs(2,5));
	ASSERT_EQ(1,dObs(3,5));
	ASSERT_EQ(1,dObs(4,5));
	ASSERT_EQ(1,dObs(5,5));
}

TEST_F(DiscretiserTest,Manually){
		Matrix<std::string> oriObs (TEST_DATA_PATH("testObservations.txt"),false,true);
	Matrix<int> dObs (oriObs.getColCount(), oriObs.getRowCount(),0);
	Network n;
	Discretiser d (oriObs,TEST_DATA_PATH("jsonDiscretiserTest.json"),dObs,n);
	
	ASSERT_EQ(0,dObs(0,6));
	ASSERT_EQ(0,dObs(1,6));
	ASSERT_EQ(0,dObs(2,6));
	ASSERT_EQ(1,dObs(3,6));
	ASSERT_EQ(1,dObs(4,6));
	ASSERT_EQ(1,dObs(5,6));
}

TEST_F(DiscretiserTest,BracketMedians){
	Matrix<std::string> oriObs (TEST_DATA_PATH("testObservations.txt"),false,true);
	Matrix<int> dObs (oriObs.getColCount(), oriObs.getRowCount(),0);
	Network n;
	Discretiser d (oriObs,TEST_DATA_PATH("jsonDiscretiserTest.json"),dObs,n);
	ASSERT_EQ(0,dObs(0,7));
	ASSERT_EQ(0,dObs(1,7));
	ASSERT_EQ(0,dObs(2,7));
	ASSERT_EQ(1,dObs(3,7));
	ASSERT_EQ(1,dObs(4,7));
	ASSERT_EQ(1,dObs(5,7));
}

TEST_F(DiscretiserTest,PerasonTukey){
	Matrix<std::string> oriObs (TEST_DATA_PATH("testObservations.txt"),false,true);
	Matrix<int> dObs (oriObs.getColCount(), oriObs.getRowCount(),0);
	Network n;
	Discretiser d (oriObs,TEST_DATA_PATH("jsonDiscretiserTest.json"),dObs,n);
	ASSERT_EQ(0,dObs(0,8));
	ASSERT_EQ(1,dObs(1,8));
	ASSERT_EQ(0,dObs(2,8));
	ASSERT_EQ(1,dObs(3,8));
	ASSERT_EQ(1,dObs(4,8));
	ASSERT_EQ(2,dObs(5,8));
}

TEST_F(DiscretiserTest,MapNamesToInt){
	Matrix<std::string> oriObs (TEST_DATA_PATH("testObservations.txt"),false,true);
	Matrix<int> dObs (oriObs.getColCount(), oriObs.getRowCount(),0);
	Network n;
	Discretiser d (oriObs,TEST_DATA_PATH("jsonDiscretiserTest.json"),dObs,n);
	ASSERT_EQ(1,dObs(0,9));
	ASSERT_EQ(0,dObs(1,9));
	ASSERT_EQ(1,dObs(2,9));
	ASSERT_EQ(1,dObs(3,9));
	ASSERT_EQ(0,dObs(4,9));
	ASSERT_EQ(0,dObs(5,9));
}

TEST_F(DiscretiserTest, Z){
	Matrix<std::string> oriObs (TEST_DATA_PATH("testObservations.txt"),false,true);
	Matrix<int> dObs (oriObs.getColCount(), oriObs.getRowCount(),0);
	Network n;
	Discretiser d (oriObs,TEST_DATA_PATH("jsonDiscretiserTest.json"),dObs,n);	
	ASSERT_EQ(0,dObs(0,10));
	ASSERT_EQ(0,dObs(1,10));
	ASSERT_EQ(1,dObs(2,10));
	ASSERT_EQ(0,dObs(3,10));
	ASSERT_EQ(0,dObs(4,10));
	ASSERT_EQ(0,dObs(5,10));
}
