#include "gtest/gtest.h"
#include "../core/NetworkController.h"
#include "../core/QueryExecuter.h"
#include "config.h"

class QueryExecuterTest : public ::testing::Test{
	protected:
	QueryExecuterTest()
		:c(NetworkController())	{
		c.loadNetwork(TEST_DATA_PATH("Student.na"));
		c.loadNetwork(TEST_DATA_PATH("Student.sif"));
//		c.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("controlStudent.txt"));
		c.trainNetwork();
	}

	public:
	NetworkController c;
	
};

TEST_F(QueryExecuterTest, QECheck01){
	QueryExecuter qe (c);
	qe.setNonIntervention(1,0);
	ASSERT_NEAR(0.362f,qe.execute().first,0.001);
}

TEST_F(QueryExecuterTest, QECheck02){
	QueryExecuter qe (c);
	qe.setNonIntervention(1,1);
	ASSERT_NEAR(0.2884f,qe.execute().first,0.001);
}

TEST_F(QueryExecuterTest, QECheck03){
	QueryExecuter qe (c);
	qe.setNonIntervention(1,2);
	ASSERT_NEAR(0.3496f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck04){
	QueryExecuter qe (c);
	qe.setNonIntervention(2,0);
	ASSERT_NEAR(0.7f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck05){
	QueryExecuter qe (c);
	qe.setNonIntervention(2,1);	
	ASSERT_NEAR(0.3f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck06){
	QueryExecuter qe (c);
	qe.setNonIntervention(0,0);
	ASSERT_NEAR(0.6f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck07){
	QueryExecuter qe (c);
	qe.setNonIntervention(0,1);
	ASSERT_NEAR(0.4f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck08){
	QueryExecuter qe (c);
	qe.setNonIntervention(3,0);
	ASSERT_NEAR(0.725f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck09){
	QueryExecuter qe (c);
	qe.setNonIntervention(3,1);
	ASSERT_NEAR(0.275f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck10){
	QueryExecuter qe (c);
	qe.setNonIntervention(4,0);
	ASSERT_NEAR(0.497664f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck11){
	QueryExecuter qe (c);
	qe.setNonIntervention(4,1);
	ASSERT_NEAR(0.502336f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck12){
	std::string query("? Grade = g1 Difficulty = d0");
	QueryExecuter qe (c);
	qe.setNonIntervention(1,0);
	qe.setNonIntervention(0,0);
	ASSERT_NEAR(0.288f,qe.execute().first,0.001);
}

TEST_F(QueryExecuterTest, QECheck13){   
	QueryExecuter qe (c);
	qe.setNonIntervention(0,0);
	qe.setNonIntervention(1,0);
	qe.setNonIntervention(2,0);
	qe.setNonIntervention(3,0);
	qe.setNonIntervention(4,0);
	ASSERT_NEAR(0.01197f,qe.execute().first,0.001);
}

TEST_F(QueryExecuterTest, QECheck14){
	QueryExecuter qe (c);
	qe.setNonIntervention(1,0);
	qe.setCondition(0,0);
	qe.setCondition(2,0);
	ASSERT_NEAR(0.3f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck15){
	QueryExecuter qe (c);
	qe.setNonIntervention(1,0);
	qe.setCondition(0,0);
	ASSERT_NEAR(0.48f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck16){
	std::string query("? Difficulty = d0 | SAT = s0");
	QueryExecuter qe (c);
	qe.setNonIntervention(0,0);
	qe.setCondition(3,0);
	ASSERT_NEAR(0.6f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck17){
	QueryExecuter qe (c);
	qe.setNonIntervention(0,0);
	qe.setCondition(1,0);
	ASSERT_NEAR(0.795f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck18){
	QueryExecuter qe (c);
	qe.setArgMax(1);
	ASSERT_NEAR(0.362f, qe.execute().first,0.001);
}

TEST_F(QueryExecuterTest, QECheck19){
	QueryExecuter qe (c);
	qe.setArgMax(3);
	ASSERT_NEAR(0.725f, qe.execute().first, 0.001);
}

TEST_F(QueryExecuterTest, QECheck20){
	QueryExecuter qe (c);
	qe.setArgMax(2);
	ASSERT_NEAR(0.7f, qe.execute().first, 0.001);	
}

TEST_F(QueryExecuterTest, QECheck21){
	QueryExecuter qe (c);
	qe.setArgMax(1);
	qe.setCondition(0,0);	
	ASSERT_NEAR(0.48f, qe.execute().first, 0.001);	
}
