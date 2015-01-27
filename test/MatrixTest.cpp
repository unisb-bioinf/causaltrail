#include "gtest/gtest.h"
#include "../Matrix.h"
#include "config.h"

class MatrixTest : public ::testing::Test{
	protected:
	MatrixTest()
	:m_(Matrix<int>(3,2,0)){
	m_.setData(1,0,0);
	m_.setData(2,0,1);
	m_.setData(3,1,0);
	m_.setData(4,1,1);
	m_.setData(5,2,0);
	m_.setData(6,2,1);
	m_.setRowNames({"A","B"});
	m_.setColNames({"A","B","C"});
	}
	public:
	Matrix<int> m_;

};

TEST_F(MatrixTest,RowSize){
	ASSERT_EQ(3u,m_.getColCount());
}

TEST_F(MatrixTest,ColSize){
	ASSERT_EQ(2u,m_.getRowCount());
}

TEST_F(MatrixTest,AccessOperator){
	ASSERT_EQ(3,m_(1,0));
}

TEST_F(MatrixTest,AccessOperatorIncrement){
	m_(1,0)++;
	ASSERT_EQ(4,m_(1,0));
}

TEST_F(MatrixTest,AccessOperatorOutOfBound){
	ASSERT_THROW(m_(5,2),std::invalid_argument);
}

TEST_F(MatrixTest,SetDataOutOfBound){
	ASSERT_THROW(m_.setData(1,5,2),std::invalid_argument);
}

TEST_F(MatrixTest,RowNames){
	ASSERT_TRUE(m_.getRowNames()[0]=="A");
	ASSERT_TRUE(m_.getRowNames()[1]=="B");

}

TEST_F(MatrixTest,ColNames){
	ASSERT_TRUE(m_.getColNames()[0]=="A");
	ASSERT_TRUE(m_.getColNames()[1]=="B");
	ASSERT_TRUE(m_.getColNames()[2]=="C");
}

TEST_F(MatrixTest,FindRow){
	ASSERT_EQ(0,m_.findRow("A"));
	ASSERT_EQ(1,m_.findRow("B"));
	ASSERT_EQ(-1,m_.findRow("Z"));
}

TEST_F(MatrixTest,FindCol){
	ASSERT_EQ(0,m_.findCol("A"));
	ASSERT_EQ(1,m_.findCol("B"));
	ASSERT_EQ(2,m_.findCol("C"));
	ASSERT_EQ(-1,m_.findCol("D"));
}

TEST_F(MatrixTest,AccessViaName){
	ASSERT_EQ(1,m_.getValueByNames("A","A"));
	ASSERT_EQ(2,m_.getValueByNames("A","B"));
}

TEST_F(MatrixTest,AccessViaNameFailure){
	ASSERT_THROW(m_.getValueByNames("D","Z"), std::invalid_argument);
}


TEST_F(MatrixTest,RowSum){
	ASSERT_EQ(9,m_.calculateRowSum(0));
	ASSERT_EQ(12,m_.calculateRowSum(1));
}

TEST_F(MatrixTest,ColSum){
	ASSERT_EQ(3,m_.calculateColSum(0));
	ASSERT_EQ(7,m_.calculateColSum(1));
	ASSERT_EQ(11,m_.calculateColSum(2));
}

TEST_F(MatrixTest,UniqueRowValues){
	ASSERT_TRUE(m_.getUniqueRowValues(0)[0]==1);
	ASSERT_TRUE(m_.getUniqueRowValues(0)[1]==3);
	ASSERT_TRUE(m_.getUniqueRowValues(0)[2]==5);
}

TEST_F(MatrixTest,UniqueColValues){
	ASSERT_TRUE(m_.getUniqueColValues(0)[0]==1);
	ASSERT_TRUE(m_.getUniqueColValues(0)[1]==2);
}

TEST_F(MatrixTest,UniqueRowValuesExcluding){
	ASSERT_TRUE(m_.getUniqueRowValues(0,3)[0]==1);
	ASSERT_TRUE(m_.getUniqueRowValues(0,3)[1]==5);
}

TEST_F(MatrixTest,UniqueColValuesExcluding){
	ASSERT_TRUE(m_.getUniqueColValues(0,1)[0]==2);
}

TEST_F(MatrixTest,containsElement){
	ASSERT_TRUE(m_.contains(1));
	ASSERT_FALSE(m_.contains(42));
}

TEST_F(MatrixTest,containsElementRow){
	ASSERT_TRUE(m_.containsElement(1,0,1));
	ASSERT_FALSE(m_.containsElement(1,0,42));
	ASSERT_TRUE(m_.containsElement(1,1,2));
	ASSERT_FALSE(m_.containsElement(1,1,44));
}

TEST_F(MatrixTest,containsElementCol){
	ASSERT_TRUE(m_.containsElement(0,0,1));
	ASSERT_TRUE(m_.containsElement(0,1,3));
	ASSERT_FALSE(m_.containsElement(0,0,42));
	ASSERT_FALSE(m_.containsElement(0,0,44));
}

TEST_F(MatrixTest,countElement){
	ASSERT_EQ(1u,m_.countElement(1,0,1));
	ASSERT_EQ(0u,m_.countElement(1,0,42));
}

TEST_F(MatrixTest,resize){
	m_.resize(10,12,0);
	ASSERT_EQ(1,m_(0,0));
	ASSERT_EQ(12u,m_.getRowCount());
	ASSERT_EQ(10u,m_.getColCount());
}

TEST_F(MatrixTest, resizeOutOfBound){
	ASSERT_THROW(m_.resize(2,2,0), std::invalid_argument);
}

TEST_F(MatrixTest,readMatrix){
	m_.readMatrix(TEST_DATA_PATH("testObservations2.txt"),false,true,0);
	ASSERT_TRUE(m_.getRowNames()[0]=="Difficulty");
	ASSERT_TRUE(m_.getRowNames()[4]=="Letter");
	ASSERT_EQ(5u,m_.getRowCount());
	ASSERT_EQ(8u,m_.getColCount());
	ASSERT_EQ(5,m_(0,0));
	ASSERT_EQ(12,m_(7,0));
	ASSERT_EQ(3,m_(2,3));
}
