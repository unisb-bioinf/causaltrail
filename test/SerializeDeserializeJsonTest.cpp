#include "gtest/gtest.h"
#include "../core/SerializeDeserializeJson.h"
#include "config.h"

class SerializeDeserializeJsonTest : public ::testing::Test{
	protected:
	SerializeDeserializeJsonTest()
	{
	}
	public:
	SerializeDeserializeJson jp;

};

TEST_F(SerializeDeserializeJsonTest,Constructor){
}

TEST_F(SerializeDeserializeJsonTest,importFile){
	ASSERT_NO_THROW(jp.importFile(TEST_DATA_PATH("jsonTest.json")));
}

TEST_F(SerializeDeserializeJsonTest,invalidFile){
	ASSERT_ANY_THROW(jp.importFile(TEST_DATA_PATH("jsonTestInvalid.json")));
	}

TEST_F(SerializeDeserializeJsonTest,containsNode){
	jp.importFile(TEST_DATA_PATH("jsonTest.json"));
	ASSERT_TRUE(jp.containsNode("Grade"));
	ASSERT_TRUE(jp.containsNode("Difficulty"));
	ASSERT_TRUE(jp.containsNode("SAT"));
	ASSERT_TRUE(jp.containsNode("Intelligence"));
	ASSERT_TRUE(jp.containsNode("Letter"));
	ASSERT_FALSE(jp.containsNode("None"));
}

TEST_F(SerializeDeserializeJsonTest,DiscretisationDetails){
	jp.importFile(TEST_DATA_PATH("jsonTest.json"));
	ASSERT_TRUE(jp.getMethod("Grade")=="Threshold");
	ASSERT_TRUE(jp.getParameter<float>("Grade","threshold")==4.5);
	ASSERT_TRUE(jp.getMethod("SAT")=="none");
	ASSERT_TRUE(jp.getMethod("Difficulty")=="BracketMedians");
	ASSERT_TRUE(jp.getParameter<int>("Difficulty","buckets")==3);
	ASSERT_TRUE(jp.getMethod("Intelligence")=="none");
	ASSERT_TRUE(jp.getMethod("Letter")=="none");
}


TEST_F(SerializeDeserializeJsonTest,addNode){
	jp.importFile(TEST_DATA_PATH("jsonTest.json"));
	SerializeDeserializeJson jp2;
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

TEST_F(SerializeDeserializeJsonTest,exportFile){
	jp.importFile(TEST_DATA_PATH("jsonTest.json"));
	jp.exportToFile(TEST_DATA_PATH("outputTest.json"));

	SerializeDeserializeJson jp2;
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
}
