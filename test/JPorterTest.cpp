#include "gtest/gtest.h"
#include "../core/JPorter.h"
#include "config.h"

class JPorterTest : public ::testing::Test{
	protected:
	JPorterTest()
	:jp(JPorter())
	{
	}
	public:
	JPorter jp;

};

TEST_F(JPorterTest,Constructor){
}

TEST_F(JPorterTest,importFile){
	ASSERT_TRUE(jp.importFile(TEST_DATA_PATH("jsonTest.json")));
}

TEST_F(JPorterTest,containsNode){
	ASSERT_TRUE(jp.importFile(TEST_DATA_PATH("jsonTest.json")));
	ASSERT_TRUE(jp.containsNode("Grade"));
	ASSERT_TRUE(jp.containsNode("Difficulty"));
	ASSERT_TRUE(jp.containsNode("SAT"));
	ASSERT_TRUE(jp.containsNode("Intelligence"));
	ASSERT_TRUE(jp.containsNode("Letter"));
	ASSERT_FALSE(jp.containsNode("None"));
}

TEST_F(JPorterTest,DiscretisationDetails){
	jp.importFile(TEST_DATA_PATH("jsonTest.json"));
	ASSERT_TRUE(jp.getDiscretisationDetails("Grade").first==6);
	ASSERT_TRUE(jp.getDiscretisationDetails("Grade").second==4.5);
	ASSERT_TRUE(jp.getDiscretisationDetails("SAT").first==9);
	ASSERT_TRUE(jp.getDiscretisationDetails("Difficulty").first==7);
	ASSERT_TRUE(jp.getDiscretisationDetails("Difficulty").second==3);
	ASSERT_TRUE(jp.getDiscretisationDetails("Intelligence").first==9);
	ASSERT_TRUE(jp.getDiscretisationDetails("Letter").first==9);	
}


TEST_F(JPorterTest,addNode){
	jp.importFile(TEST_DATA_PATH("jsonTest.json"));
	JPorter jp2;
	jp2.addToTree("Difficulty",7,3.0);
	jp2.addToTree("Grade","Threshold",4.5);
	jp2.addToTree("Intelligence","none");
	jp2.addToTree("SAT","none");
	jp2.addToTree("Letter","none");
	ASSERT_TRUE(jp2.containsNode("Grade"));
	ASSERT_TRUE(jp2.containsNode("Difficulty"));
	ASSERT_TRUE(jp2.containsNode("SAT"));
	ASSERT_TRUE(jp2.containsNode("Intelligence"));
	ASSERT_TRUE(jp2.containsNode("Letter"));
	ASSERT_FALSE(jp2.containsNode("None"));
	ASSERT_TRUE(jp2.getDiscretisationDetails("Grade").first==6);
	ASSERT_TRUE(jp2.getDiscretisationDetails("Grade").second==4.5);
	ASSERT_TRUE(jp2.getDiscretisationDetails("SAT").first==9);
	ASSERT_TRUE(jp2.getDiscretisationDetails("Difficulty").first==7);
	ASSERT_TRUE(jp2.getDiscretisationDetails("Difficulty").second==3);
	ASSERT_TRUE(jp2.getDiscretisationDetails("Intelligence").first==9);
	ASSERT_TRUE(jp2.getDiscretisationDetails("Letter").first==9);	

}

TEST_F(JPorterTest,importInvalidFile){
	ASSERT_FALSE(jp.importFile(TEST_DATA_PATH("jsonTestInvalid.json")));
}

TEST_F(JPorterTest,exportFile){
	jp.importFile(TEST_DATA_PATH("jsonTest.json"));
	jp.exportToFile(TEST_DATA_PATH("outputTest.json"));
	JPorter jp2;
	jp2.importFile(TEST_DATA_PATH("outputTest.json"));
	ASSERT_TRUE(jp.getPropertyTree() == jp2.getPropertyTree());
}

TEST_F(JPorterTest,convertNameToInteger){
	ASSERT_TRUE(jp.convertMethodToIntegerCode("Ceil")==0);
	ASSERT_TRUE(jp.convertMethodToIntegerCode("Floor")==1);
	ASSERT_TRUE(jp.convertMethodToIntegerCode("Round")==2);
	ASSERT_TRUE(jp.convertMethodToIntegerCode("ArithmeticMean")==3);
	ASSERT_TRUE(jp.convertMethodToIntegerCode("HarmonicMean")==4);
	ASSERT_TRUE(jp.convertMethodToIntegerCode("Median")==5);
	ASSERT_TRUE(jp.convertMethodToIntegerCode("Threshold")==6);
	ASSERT_TRUE(jp.convertMethodToIntegerCode("BracketMedians")==7);
	ASSERT_TRUE(jp.convertMethodToIntegerCode("PearsonTukey")==8);
	ASSERT_TRUE(jp.convertMethodToIntegerCode("none")==9);
	ASSERT_TRUE(jp.convertMethodToIntegerCode("Z-Score")==10);
}

TEST_F(JPorterTest,convertNameToIntegerInvalid){
	ASSERT_THROW(jp.convertMethodToIntegerCode("None"),std::invalid_argument);
}

