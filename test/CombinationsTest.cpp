#include "gtest/gtest.h"
#include "../Combinations.h"

class CombinationsTest : public ::testing::Test{
	protected:
	CombinationsTest()
	{
	}

	void virtual SetUp(){
		mapInt[0]=valuesInt;
		mapInt[1]=valuesInt2;
		mapString[0]=valuesString;
		mapString[1]=valuesString2;
	}

	public:
	std::vector<unsigned int> keys = {0, 1};
	std::vector<int> valuesInt = {1,2};
	std::vector<int> valuesInt2 = {1,2,3};
	std::vector<std::string> valuesString = {"A","B"};
	std::vector<std::string> valuesString2 = {"A","B","C"};
	std::unordered_map<unsigned int, std::vector<int>> mapInt;
	std::unordered_map<unsigned int, std::vector<std::string>> mapString;
	
};

TEST_F(CombinationsTest,Int){
	Combinations<int> c(keys,mapInt);
	c.createCombinations();
	std::vector<std::vector<int>> result = c.getResult();
	ASSERT_EQ(6,result.size());
	ASSERT_EQ(1,result[0][0]);
	ASSERT_EQ(1,result[0][1]);
	ASSERT_EQ(1,result[1][0]);
	ASSERT_EQ(2,result[1][1]);
	ASSERT_EQ(1,result[2][0]);
	ASSERT_EQ(3,result[2][1]);
	ASSERT_EQ(2,result[3][0]);
	ASSERT_EQ(1,result[3][1]);
	ASSERT_EQ(2,result[4][0]);
	ASSERT_EQ(2,result[4][1]);
	ASSERT_EQ(2,result[5][0]);
	ASSERT_EQ(3,result[5][1]);
}

TEST_F(CombinationsTest,String){
	Combinations<std::string> c(keys,mapString);
	c.createCombinations();
	std::vector<std::vector<std::string>> result = c.getResult();
	ASSERT_EQ(6,result.size());
	ASSERT_EQ("A",result[0][0]);
	ASSERT_EQ("A",result[0][1]);
	ASSERT_EQ("A",result[1][0]);
	ASSERT_EQ("B",result[1][1]);
	ASSERT_EQ("A",result[2][0]);
	ASSERT_EQ("C",result[2][1]);
	ASSERT_EQ("B",result[3][0]);
	ASSERT_EQ("A",result[3][1]);
	ASSERT_EQ("B",result[4][0]);
	ASSERT_EQ("B",result[4][1]);
	ASSERT_EQ("B",result[5][0]);
	ASSERT_EQ("C",result[5][1]);
}
