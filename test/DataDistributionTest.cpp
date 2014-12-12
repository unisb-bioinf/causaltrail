#include "gtest/gtest.h"
#include "../NetworkController.h"
#include "config.h"

class DataDistributionTest : public ::testing::Test{
	protected:
	DataDistributionTest()
	{

	}
};

TEST_F(DataDistributionTest, Constructor){
	Network n;
	Matrix<int> ob (0,0,0);
	DataDistribution db (n, ob);
	SUCCEED();
}

TEST_F(DataDistributionTest, assignObservationsToNodesTestEmpty){
	Network n;
	Matrix<int> ob (0,0,0);
	DataDistribution db (n, ob);
	db.assignObservationsToNodes();
	SUCCEED();
}

TEST_F(DataDistributionTest, distributeObservationsTestEmpty){
	Network n;
	Matrix<int> ob (0,0,0);
	DataDistribution db (n, ob);
	db.assignObservationsToNodes();
	db.distributeObservations();
	SUCCEED();
}

TEST_F(DataDistributionTest, assignObservationsToNodesTestStudentComplet){
	Network n;
	n.readNetwork(TEST_DATA_PATH("Student.na"));
	n.readNetwork(TEST_DATA_PATH("Student.sif"));
	Matrix<std::string> originalObservations (TEST_DATA_PATH("StudentData.txt"),false,true,"NA");
	Matrix<int> observations (0,0,-1);
	Discretiser disc = Discretiser(originalObservations,TEST_DATA_PATH("controlStudent.txt"), observations, n);
	DataDistribution db (n, observations);
	db.assignObservationsToNodes();
	std::vector<std::string> empty {"1"};

	Node& intel = n.getNode("Intelligence");
	std::vector<int> uniqueValuesi {0,1};
	std::vector<std::string> uniqueNamesi {"i0","i1"};
	ASSERT_TRUE(1 == intel.getObservationRow());
	ASSERT_TRUE(uniqueValuesi == intel.getUniqueValues());
	ASSERT_TRUE(uniqueValuesi == intel.getUniqueValuesExcludingNA());
	ASSERT_TRUE(1 == intel.getNumberOfParentValues());
	ASSERT_TRUE(uniqueNamesi == intel.getValueNames());
	ASSERT_TRUE(empty == intel.getParentValueNames());

	Node& dif = n.getNode("Difficulty");
	std::vector<int> uniqueValuesd {0,1};
	std::vector<std::string> uniqueNamesd {"d0","d1"};
	ASSERT_TRUE(0 == dif.getObservationRow());
	ASSERT_TRUE(uniqueValuesd == dif.getUniqueValues());
	ASSERT_TRUE(uniqueValuesd == dif.getUniqueValuesExcludingNA());
	ASSERT_TRUE(1 == dif.getNumberOfParentValues());
	ASSERT_TRUE(uniqueNamesd == dif.getValueNames());
	ASSERT_TRUE(empty == dif.getParentValueNames());

	Node& grade = n.getNode("Grade");
	std::vector<int> uniqueValuesg {0,1,2};
	std::vector<std::string> uniqueNamesg {"g1","g2","g3"};
	std::vector<std::string> parentValueNamesg {"d0,i0","d0,i1","d1,i0","d1,i1"};
	ASSERT_TRUE(3 == grade.getObservationRow());
	ASSERT_TRUE(uniqueValuesg == grade.getUniqueValues());
	ASSERT_TRUE(uniqueValuesg == grade.getUniqueValuesExcludingNA());
	ASSERT_TRUE(4 == grade.getNumberOfParentValues());
	ASSERT_TRUE(uniqueNamesg == grade.getValueNames());
	ASSERT_TRUE(parentValueNamesg == grade.getParentValueNames());

	Node& letter = n.getNode("Letter");
	std::vector<int> uniqueValuesl {0,1};
	std::vector<std::string> uniqueNamesl {"l0","l1"};
	std::vector<std::string> parentValueNamesl {"g1","g2","g3"};
	ASSERT_TRUE(4 == letter.getObservationRow());
	ASSERT_TRUE(uniqueValuesl == letter.getUniqueValues());
	ASSERT_TRUE(uniqueValuesl == letter.getUniqueValuesExcludingNA());
	ASSERT_TRUE(3 == letter.getNumberOfParentValues());
	ASSERT_TRUE(uniqueNamesl == letter.getValueNames());
	ASSERT_TRUE(parentValueNamesl == letter.getParentValueNames());

	Node& sat = n.getNode("SAT");
	std::vector<int> uniqueValuess {0,1};
	std::vector<std::string> uniqueNamess {"s0","s1"};
	std::vector<std::string> parentValueNamess {"i0","i1"};
	ASSERT_TRUE(2 == sat.getObservationRow());
	ASSERT_TRUE(uniqueValuess == sat.getUniqueValues());
	ASSERT_TRUE(uniqueValuess == sat.getUniqueValuesExcludingNA());
	ASSERT_TRUE(2 == sat.getNumberOfParentValues());
	ASSERT_TRUE(uniqueNamess == sat.getValueNames());
	ASSERT_TRUE(parentValueNamess == sat.getParentValueNames());
}

TEST_F(DataDistributionTest, assignObservationsToNodesTestStudentInComplet){
	Network n;
	n.readNetwork(TEST_DATA_PATH("Student.na"));
	n.readNetwork(TEST_DATA_PATH("Student.sif"));
	Matrix<std::string> originalObservations (TEST_DATA_PATH("dataStudent60.txt"),false,true,"NA");
	Matrix<int> observations (0,0,-1);
	Discretiser disc = Discretiser(originalObservations,TEST_DATA_PATH("controlStudent.txt"), observations, n);
	DataDistribution db (n, observations);
	db.assignObservationsToNodes();
	std::vector<std::string> empty {"1"};

	Node& intel = n.getNode("Intelligence");
	std::vector<int> uniqueValuesi {0,1};
	std::vector<std::string> uniqueNamesi {"i0","i1"};
	ASSERT_TRUE(1 == intel.getObservationRow());
	ASSERT_TRUE(uniqueValuesi != intel.getUniqueValues());
	ASSERT_TRUE(uniqueValuesi == intel.getUniqueValuesExcludingNA());
	ASSERT_TRUE(1 == intel.getNumberOfParentValues());
	ASSERT_TRUE(uniqueNamesi == intel.getValueNamesProb ());
	ASSERT_TRUE(empty == intel.getParentValueNames());

	Node& dif = n.getNode("Difficulty");
	std::vector<int> uniqueValuesd {0,1};
	std::vector<std::string> uniqueNamesd {"d0","d1"};
	ASSERT_TRUE(0 == dif.getObservationRow());
	ASSERT_TRUE(uniqueValuesd != dif.getUniqueValues());
	ASSERT_TRUE(uniqueValuesd == dif.getUniqueValuesExcludingNA());
	ASSERT_TRUE(1 == dif.getNumberOfParentValues());
	ASSERT_TRUE(uniqueNamesd == dif.getValueNamesProb());
	ASSERT_TRUE(empty == dif.getParentValueNames());

	Node& grade = n.getNode("Grade");
	std::vector<int> uniqueValuesg {0,1,2};
	std::vector<std::string> uniqueNamesg {"g1","g2","g3"};
	std::vector<std::string> parentValueNamesg {"d0,i0","d0,i1","d1,i0","d1,i1"};
	ASSERT_TRUE(3 == grade.getObservationRow());
	ASSERT_TRUE(uniqueValuesg != grade.getUniqueValues());
	ASSERT_TRUE(uniqueValuesg == grade.getUniqueValuesExcludingNA());
	ASSERT_TRUE(4 == grade.getNumberOfParentValues());
	ASSERT_TRUE(uniqueNamesg == grade.getValueNamesProb());
	ASSERT_TRUE(parentValueNamesg == grade.getParentValueNames());

	Node& letter = n.getNode("Letter");
	std::vector<int> uniqueValuesl {0,1};
	std::vector<std::string> uniqueNamesl {"l0","l1"};
	std::vector<std::string> parentValueNamesl {"g1","g2","g3"};
	ASSERT_TRUE(4 == letter.getObservationRow());
	ASSERT_TRUE(uniqueValuesl != letter.getUniqueValues());
	ASSERT_TRUE(uniqueValuesl == letter.getUniqueValuesExcludingNA());
	ASSERT_TRUE(3 == letter.getNumberOfParentValues());
	ASSERT_TRUE(uniqueNamesl == letter.getValueNamesProb());
	ASSERT_TRUE(parentValueNamesl == letter.getParentValueNames());

	Node& sat = n.getNode("SAT");
	std::vector<int> uniqueValuess {0,1};
	std::vector<std::string> uniqueNamess {"s0","s1"};
	std::vector<std::string> parentValueNamess {"i0","i1"};
	ASSERT_TRUE(2 == sat.getObservationRow());
	ASSERT_TRUE(uniqueValuess != sat.getUniqueValues());
	ASSERT_TRUE(uniqueValuess == sat.getUniqueValuesExcludingNA());
	ASSERT_TRUE(2 == sat.getNumberOfParentValues());
	ASSERT_TRUE(uniqueNamess == sat.getValueNamesProb());
	ASSERT_TRUE(parentValueNamess == sat.getParentValueNames());
}

TEST_F(DataDistributionTest, distributeObservationsTestStudentComplet){
	Network n;
	n.readNetwork(TEST_DATA_PATH("Student.na"));
	n.readNetwork(TEST_DATA_PATH("Student.sif"));
	Matrix<std::string> originalObservations (TEST_DATA_PATH("StudentData.txt"),false,true,"NA");
	Matrix<int> observations (0,0,-1);
	Discretiser disc = Discretiser(originalObservations,TEST_DATA_PATH("controlStudent.txt"), observations, n);
	DataDistribution db (n, observations);
	db.assignObservationsToNodes();
	db.distributeObservations();
	Node dif = n.getNode("Difficulty");
	ASSERT_EQ(60000,dif.getObservations(0,0));
	ASSERT_EQ(40000,dif.getObservations(1,0));

	Node& intel = n.getNode("Intelligence");
	ASSERT_EQ(70000,intel.getObservations(0,0));
	ASSERT_EQ(30000,intel.getObservations(1,0));

	Node& grade = n.getNode("Grade");
	ASSERT_EQ(13500,grade.getObservations(0,0));
	ASSERT_EQ(1250,grade.getObservations(0,2));
	ASSERT_EQ(13500,grade.getObservations(0,1));
	ASSERT_EQ(7500,grade.getObservations(0,3));
	ASSERT_EQ(18000,grade.getObservations(1,0));
	ASSERT_EQ(6250,grade.getObservations(1,2));
	ASSERT_EQ(1200,grade.getObservations(1,1));
	ASSERT_EQ(4500,grade.getObservations(1,3));
	ASSERT_EQ(13500,grade.getObservations(2,0));
	ASSERT_EQ(17500,grade.getObservations(2,2));
	ASSERT_EQ(300,grade.getObservations(2,1));
	ASSERT_EQ(3000,grade.getObservations(2,3));

	Node& SAT = n.getNode("SAT");
	ASSERT_EQ(66500,SAT.getObservations(0,0));
	ASSERT_EQ(6000,SAT.getObservations(0,1));
	ASSERT_EQ(3500,SAT.getObservations(1,0));
	ASSERT_EQ(24000,SAT.getObservations(1,1));
	
	Node& Letter = n.getNode("Letter");
	ASSERT_EQ(3575 , Letter.getObservations(0,0));
	ASSERT_EQ(11980 , Letter.getObservations(0,1));
	ASSERT_EQ(33957 , Letter.getObservations(0,2));
	ASSERT_EQ(32175 , Letter.getObservations(1,0));
	ASSERT_EQ(17970 , Letter.getObservations(1,1));
	ASSERT_EQ(343 , Letter.getObservations(1,2));
}

TEST_F(DataDistributionTest, distributeObservationsTestStudentInComplet){
	Network n;
	n.readNetwork(TEST_DATA_PATH("Student.na"));
	n.readNetwork(TEST_DATA_PATH("Student.sif"));
	Matrix<std::string> originalObservations (TEST_DATA_PATH("dataStudent60.txt"),false,true,"NA");
	Matrix<int> observations (0,0,-1);
	Discretiser disc = Discretiser(originalObservations,TEST_DATA_PATH("controlStudent.txt"), observations, n);
	DataDistribution db (n, observations);
	db.assignObservationsToNodes();
	
	db.distributeObservations();
	Node dif = n.getNode("Difficulty");
	ASSERT_EQ(59999,dif.getObservations(0,0));
	
	ASSERT_EQ(23973,dif.getObservations(1,0));
	ASSERT_EQ(16028,dif.getObservations(2,0));

	Node& intel = n.getNode("Intelligence");
	ASSERT_EQ(59999,intel.getObservations(0,0));
	
	ASSERT_EQ(27829,intel.getObservations(1,0));
	ASSERT_EQ(12172,intel.getObservations(2,0));

	Node& grade = n.getNode("Grade");
	ASSERT_EQ(4234,grade.getObservations(0,0));
	ASSERT_EQ(1461,grade.getObservations(0,1));
	ASSERT_EQ(2395,grade.getObservations(0,2));
	ASSERT_EQ(1444,grade.getObservations(0,3));
	
	ASSERT_EQ(914,grade.getObservations(1,0));
	ASSERT_EQ(94,grade.getObservations(1,2));
	ASSERT_EQ(924,grade.getObservations(1,1));
	ASSERT_EQ(465,grade.getObservations(1,3));
	ASSERT_EQ(1136,grade.getObservations(2,0));
	ASSERT_EQ(84,grade.getObservations(2,1));
	ASSERT_EQ(399,grade.getObservations(2,2));
	ASSERT_EQ(304,grade.getObservations(2,3));
	ASSERT_EQ(840,grade.getObservations(3,0));
	ASSERT_EQ(12,grade.getObservations(3,1));
	ASSERT_EQ(1081,grade.getObservations(3,2));
	ASSERT_EQ(205,grade.getObservations(3,3));

	Node& SAT = n.getNode("SAT");
	ASSERT_EQ(16641,SAT.getObservations(0,0));
	ASSERT_EQ(7332,SAT.getObservations(0,1));

	ASSERT_EQ(10601,SAT.getObservations(1,0));
	ASSERT_EQ(1018,SAT.getObservations(1,1));
	ASSERT_EQ(587,SAT.getObservations(2,0));
	ASSERT_EQ(3822,SAT.getObservations(2,1));
	
	Node& Letter = n.getNode("Letter");
	ASSERT_EQ(8570 , Letter.getObservations(0,0));
	ASSERT_EQ(7136 , Letter.getObservations(0,1));
	ASSERT_EQ(8266 , Letter.getObservations(0,2));
	
	ASSERT_EQ(547 , Letter.getObservations(1,0));
	ASSERT_EQ(1856 , Letter.getObservations(1,1));
	ASSERT_EQ(5494 , Letter.getObservations(1,2));
	ASSERT_EQ(5206 , Letter.getObservations(2,0));
	ASSERT_EQ(2875 , Letter.getObservations(2,1));
	ASSERT_EQ(51, Letter.getObservations(2,2));
}
