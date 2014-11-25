#include "gtest/gtest.h"
#include "../NetworkController.h"
#include "../EM.h"

class EMTest : public ::testing::Test{
	protected:
	EMTest()
		:c(NetworkController())
	{
	}


	public:
	NetworkController c;
	
};

TEST_F(EMTest,Complete){
	c.loadNetwork("Student.na");
	c.loadNetwork("Student.sif");
	c.loadObservations("StudentData.txt","controlStudent.txt");
	c.trainNetwork();
	Network n = c.getNetwork();
	Node grade = n.getNode("Grade");
	ASSERT_NEAR(0.3f,grade.getProbability(0,0),0.001);
	ASSERT_NEAR(0.05f,grade.getProbability(0,2),0.001);
	ASSERT_NEAR(0.9f,grade.getProbability(0,1),0.001);
	ASSERT_NEAR(0.5f,grade.getProbability(0,3),0.001);
	ASSERT_NEAR(0.4f,grade.getProbability(1,0),0.001);
	ASSERT_NEAR(0.25f,grade.getProbability(1,2),0.001);
	ASSERT_NEAR(0.08f,grade.getProbability(1,1),0.001);
	ASSERT_NEAR(0.3f,grade.getProbability(1,3),0.001);
	ASSERT_NEAR(0.3f,grade.getProbability(2,0),0.001);
	ASSERT_NEAR(0.7f,grade.getProbability(2,2),0.001);
	ASSERT_NEAR(0.02f,grade.getProbability(2,1),0.001);
	ASSERT_NEAR(0.2f,grade.getProbability(2,3),0.001);
	Node letter = n.getNode("Letter");
	ASSERT_NEAR(0.1f,letter.getProbability(0,0),0.001);
	ASSERT_NEAR(0.4f,letter.getProbability(0,1),0.001);
	ASSERT_NEAR(0.99f,letter.getProbability(0,2),0.001);
	ASSERT_NEAR(0.9f,letter.getProbability(1,0),0.001);
	ASSERT_NEAR(0.6f,letter.getProbability(1,1),0.001);
	ASSERT_NEAR(0.01f,letter.getProbability(1,2),0.001);
	Node intelligence = n.getNode("Intelligence");
	ASSERT_NEAR(0.7, intelligence.getProbability(0,0),0.001);
	ASSERT_NEAR(0.3, intelligence.getProbability(1,0),0.001);
	Node difficulty = n.getNode("Difficulty");
	ASSERT_NEAR(0.6, difficulty.getProbability(0,0),0.001);
	ASSERT_NEAR(0.4, difficulty.getProbability(1,0),0.001);	
	Node sat = n.getNode("SAT");
	ASSERT_NEAR(0.95f,sat.getProbability(0,0),0.001);
	ASSERT_NEAR(0.05f,sat.getProbability(1,0),0.001);
	ASSERT_NEAR(0.2f,sat.getProbability(0,1),0.001);
	ASSERT_NEAR(0.8f,sat.getProbability(1,1),0.001);
	
}

TEST_F(EMTest,UnComplete){
	c.loadNetwork("Student.na");
	c.loadNetwork("Student.sif");
	c.loadObservations("dataStudent60.txt","controlStudent.txt");
	c.trainNetwork();
	Network n = c.getNetwork();
	Node grade = n.getNode("Grade");
	ASSERT_NEAR(0.3f,grade.getProbability(0,0),0.2);
	ASSERT_NEAR(0.05f,grade.getProbability(0,2),0.2);
	ASSERT_NEAR(0.9f,grade.getProbability(0,1),0.2);
	ASSERT_NEAR(0.5f,grade.getProbability(0,3),0.2);
	ASSERT_NEAR(0.4f,grade.getProbability(1,0),0.2);
	ASSERT_NEAR(0.25f,grade.getProbability(1,2),0.2);
	ASSERT_NEAR(0.08f,grade.getProbability(1,1),0.2);
	ASSERT_NEAR(0.3f,grade.getProbability(1,3),0.2);
	ASSERT_NEAR(0.3f,grade.getProbability(2,0),0.2);
	ASSERT_NEAR(0.7f,grade.getProbability(2,2),0.2);
	ASSERT_NEAR(0.02f,grade.getProbability(2,1),0.2);
	ASSERT_NEAR(0.2f,grade.getProbability(2,3),0.2);
	Node letter = n.getNode("Letter");
	ASSERT_NEAR(0.1f,letter.getProbability(0,0),0.2);
	ASSERT_NEAR(0.4f,letter.getProbability(0,1),0.2);
	ASSERT_NEAR(0.99f,letter.getProbability(0,2),0.2);
	ASSERT_NEAR(0.9f,letter.getProbability(1,0),0.2);
	ASSERT_NEAR(0.6f,letter.getProbability(1,1),0.2);
	ASSERT_NEAR(0.01f,letter.getProbability(1,2),0.2);
	Node intelligence = n.getNode("Intelligence");
	ASSERT_NEAR(0.7, intelligence.getProbability(0,0),0.2);
	ASSERT_NEAR(0.3, intelligence.getProbability(1,0),0.2);
	Node difficulty = n.getNode("Difficulty");
	ASSERT_NEAR(0.6, difficulty.getProbability(0,0),0.2);
	ASSERT_NEAR(0.4, difficulty.getProbability(1,0),0.2);	
	Node sat = n.getNode("SAT");
	ASSERT_NEAR(0.95f,sat.getProbability(0,0),0.2);
	ASSERT_NEAR(0.05f,sat.getProbability(1,0),0.2);
	ASSERT_NEAR(0.2f,sat.getProbability(0,1),0.2);
	ASSERT_NEAR(0.8f,sat.getProbability(1,1),0.2);
}
