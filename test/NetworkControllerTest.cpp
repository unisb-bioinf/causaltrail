#include "gtest/gtest.h"
#include "../NetworkController.h"
#include "config.h"

class NetworkControllerTest : public ::testing::Test{
	protected:
	NetworkControllerTest()
	{

	}
};

TEST_F(NetworkControllerTest, Constructor){
	NetworkController n ();
	SUCCEED();
}

TEST_F(NetworkControllerTest, ConstructorTGF){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("test.tgf"));
	SUCCEED();
}

TEST_F(NetworkControllerTest, ConstructorSIFNA){
	NetworkController n;
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("Student.sif")),std::invalid_argument);
}

TEST_F(NetworkControllerTest, ConstructorNASIF){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	n.loadNetwork(TEST_DATA_PATH("Student.sif"));
	SUCCEED();	
}

TEST_F(NetworkControllerTest, getNetwork){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	n.loadNetwork(TEST_DATA_PATH("Student.sif"));
	n.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("controlStudent.txt"));
	ASSERT_TRUE(5 == n.getNetwork().size());
}

TEST_F(NetworkControllerTest, InvalidFileName1){
	NetworkController n;
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("unkownfile.tgf")),std::invalid_argument);
}

TEST_F(NetworkControllerTest, InvalidFileName2){
	NetworkController n;
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("unkownfile.na")),std::invalid_argument);
}

TEST_F(NetworkControllerTest, InvalidFileName3){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("unkownfile.sif")),std::invalid_argument);
}

TEST_F(NetworkControllerTest, InvalidFileTyp){
	NetworkController n;
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("unkown.txt")),std::invalid_argument);
}

TEST_F(NetworkControllerTest, InvalidFileTGF){
	NetworkController n;
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("invalid.tgf")), std::invalid_argument);
}

TEST_F(NetworkControllerTest, InvalidFileTGFNodes){
	NetworkController n;
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("invalidNodeID.tgf")), std::invalid_argument);
}

TEST_F(NetworkControllerTest, InvalidFileTGFEdges){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("invalidEdge.tgf")), std::invalid_argument);
}

TEST_F(NetworkControllerTest, InvalidNA1){
	NetworkController n;
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("invalid1.na")), std::invalid_argument);
}

TEST_F(NetworkControllerTest, InvalidNA2){
	NetworkController n;
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("invalid2.na")), std::invalid_argument);
}

TEST_F(NetworkControllerTest, InvalidSIF1){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("invalid1.sif")), std::invalid_argument);
}

TEST_F(NetworkControllerTest, InvalidSIF2){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	ASSERT_THROW(n.loadNetwork(TEST_DATA_PATH("invalid2.sif")), std::invalid_argument);
}

TEST_F(NetworkControllerTest, loadObservations){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	n.loadNetwork(TEST_DATA_PATH("Student.sif"));
	n.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("controlStudent.txt"));
	SUCCEED();
}

TEST_F(NetworkControllerTest, loadObservationsInvalidObservationFileName){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	n.loadNetwork(TEST_DATA_PATH("Student.sif"));
	ASSERT_THROW(n.loadObservations(TEST_DATA_PATH("StudentDat.txt"),TEST_DATA_PATH("controlStudent.txt")),std::invalid_argument);
}

TEST_F(NetworkControllerTest, loadObservationsInvalidControlFileName){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	n.loadNetwork(TEST_DATA_PATH("Student.sif"));
	ASSERT_THROW(n.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("controlStud.txt")), std::invalid_argument);
}

TEST_F(NetworkControllerTest, loadObservationsInvalidControlFile1){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	n.loadNetwork(TEST_DATA_PATH("Student.sif"));
	ASSERT_THROW(n.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("invalidControl1.txt")), std::invalid_argument);
}

TEST_F(NetworkControllerTest, loadObservationsInvalidControlFile2){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	n.loadNetwork(TEST_DATA_PATH("Student.sif"));
	ASSERT_THROW(n.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("invalidControl2.txt")), std::invalid_argument);
}

TEST_F(NetworkControllerTest, trainNet){
	NetworkController nc;
	nc.loadNetwork(TEST_DATA_PATH("Student.na"));
	nc.loadNetwork(TEST_DATA_PATH("Student.sif"));
	nc.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("controlStudent.txt"));
	nc.trainNetwork();
	Network n = nc.getNetwork();
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

TEST_F(NetworkControllerTest, Runs){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	n.loadNetwork(TEST_DATA_PATH("Student.sif"));
	n.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("controlStudent.txt"));
	n.trainNetwork();
	ASSERT_TRUE(1==n.getNumberOfEMRuns());
}

TEST_F(NetworkControllerTest, Time){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	n.loadNetwork(TEST_DATA_PATH("Student.sif"));
	n.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("controlStudent.txt"));
	n.trainNetwork();
	ASSERT_TRUE(n.getTimeInMicroSeconds() > 0);
}

TEST_F(NetworkControllerTest, ParamDifference){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	n.loadNetwork(TEST_DATA_PATH("Student.sif"));
	n.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("controlStudent.txt"));
	n.trainNetwork();
	ASSERT_TRUE(n.getParameterDifference() > 0.0f);
}

TEST_F(NetworkControllerTest, DataLikelihood){
	NetworkController n;
	n.loadNetwork(TEST_DATA_PATH("Student.na"));
	n.loadNetwork(TEST_DATA_PATH("Student.sif"));
	n.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("controlStudent.txt"));
	ASSERT_TRUE(n.getLikelihoodOfTheData() != 0.0f);
}


