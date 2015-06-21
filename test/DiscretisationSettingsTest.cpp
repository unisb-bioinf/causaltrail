#include "gtest/gtest.h"
#include "../core/DiscretisationSettings.h"
#include "config.h"
#include <stdio.h>

class DiscretisationSettingsTest : public ::testing::Test{
	protected:
	DiscretisationSettingsTest()
	{
	}
	public:
	DiscretisationSettings jp;

};

TEST_F(DiscretisationSettingsTest,Constructor){
}

TEST_F(DiscretisationSettingsTest,importFile){
	ASSERT_NO_THROW(jp.importFile(TEST_DATA_PATH("jsonTest.json")));
}

TEST_F(DiscretisationSettingsTest,invalidFile){
	ASSERT_ANY_THROW(jp.importFile(TEST_DATA_PATH("jsonTestInvalid.json")));
	}

TEST_F(DiscretisationSettingsTest,containsNode){
	jp.importFile(TEST_DATA_PATH("jsonTest.json"));
	ASSERT_TRUE(jp.containsNode("Grade"));
	ASSERT_TRUE(jp.containsNode("Difficulty"));
	ASSERT_TRUE(jp.containsNode("SAT"));
	ASSERT_TRUE(jp.containsNode("Intelligence"));
	ASSERT_TRUE(jp.containsNode("Letter"));
	ASSERT_FALSE(jp.containsNode("None"));
}

TEST_F(DiscretisationSettingsTest,DiscretisationDetails){
	jp.importFile(TEST_DATA_PATH("jsonTest.json"));
	ASSERT_TRUE(jp.getMethod("Grade")=="Threshold");
	ASSERT_TRUE(jp.getParameter<float>("Grade","threshold")==4.5);
	ASSERT_TRUE(jp.getMethod("SAT")=="none");
	ASSERT_TRUE(jp.getMethod("Difficulty")=="BracketMedians");
	ASSERT_TRUE(jp.getParameter<int>("Difficulty","buckets")==3);
	ASSERT_TRUE(jp.getMethod("Intelligence")=="none");
	ASSERT_TRUE(jp.getMethod("Letter")=="none");
}


TEST_F(DiscretisationSettingsTest,addNode){
	jp.importFile(TEST_DATA_PATH("jsonTest.json"));
	DiscretisationSettings jp2;
	jp2.addToTree("Difficulty","BracketMedians","buckets","3");
	jp2.addToTree("Grade","Threshold","threshold","4.5");
	jp2.addToTree("Intelligence","none");
	jp2.addToTree("SAT","none");
	jp2.addToTree("Letter","none");
	ASSERT_TRUE(jp2.containsNode("Grade"));
	ASSERT_TRUE(jp2.containsNode("Difficulty"));
	ASSERT_TRUE(jp2.containsNode("SAT"));
	ASSERT_TRUE(jp2.containsNode("Intelligence"));
	ASSERT_TRUE(jp2.containsNode("Letter"));
	ASSERT_FALSE(jp2.containsNode("None"));
	ASSERT_TRUE(jp.getMethod("Grade")=="Threshold");
	ASSERT_TRUE(jp.getParameter<float>("Grade","threshold")==4.5);
	ASSERT_TRUE(jp.getMethod("SAT")=="none");
	ASSERT_TRUE(jp.getMethod("Difficulty")=="BracketMedians");
	ASSERT_TRUE(jp.getParameter<int>("Difficulty","buckets")==3);
	ASSERT_TRUE(jp.getMethod("Intelligence")=="none");
	ASSERT_TRUE(jp.getMethod("Letter")=="none");
	
}

TEST_F(DiscretisationSettingsTest,exportFile){
	jp.importFile(TEST_DATA_PATH("jsonTest.json"));
	jp.exportToFile(TEST_DATA_PATH("outputTest.json"));

	DiscretisationSettings jp2;
	jp2.importFile(TEST_DATA_PATH("outputTest.json"));
	ASSERT_TRUE(jp.containsNode("Grade"));
	ASSERT_TRUE(jp.containsNode("Difficulty"));
	ASSERT_TRUE(jp.containsNode("SAT"));
	ASSERT_TRUE(jp.containsNode("Intelligence"));
	ASSERT_TRUE(jp.containsNode("Letter"));
	ASSERT_FALSE(jp.containsNode("None"));
	ASSERT_TRUE(jp.getMethod("Grade")=="Threshold");
	ASSERT_TRUE(jp.getParameter<float>("Grade","threshold")==4.5);
	ASSERT_TRUE(jp.getMethod("SAT")=="none");
	ASSERT_TRUE(jp.getMethod("Difficulty")=="BracketMedians");
	ASSERT_TRUE(jp.getParameter<int>("Difficulty","buckets")==3);
	ASSERT_TRUE(jp.getMethod("Intelligence")=="none");
	ASSERT_TRUE(jp.getMethod("Letter")=="none");
	
	remove(TEST_DATA_PATH("outputTest.json"));
}
