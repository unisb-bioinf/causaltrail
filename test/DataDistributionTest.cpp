#include "gtest/gtest.h"
#include "../core/NetworkController.h"
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
	Matrix<std::string> originalObservations (TEST_DATA_PATH("StudentData.txt"),false,true);
	Matrix<int> observations (0,0,-1);
	Discretiser disc(originalObservations,TEST_DATA_PATH("controlStudent.txt"), observations, n);
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
	Matrix<std::string> originalObservations (TEST_DATA_PATH("dataStudent60.txt"),false,true);
	Matrix<int> observations (0,0,-1);
	Discretiser disc(originalObservations,TEST_DATA_PATH("controlStudent.txt"), observations, n);
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
	Matrix<std::string> originalObservations (TEST_DATA_PATH("StudentData.txt"),false,true);
	Matrix<int> observations (0,0,-1);
	Discretiser disc(originalObservations,TEST_DATA_PATH("controlStudent.txt"), observations, n);
	DataDistribution db (n, observations);
	db.assignObservationsToNodes();
	db.distributeObservations();
	Node dif = n.getNode("Difficulty");
	ASSERT_EQ(60000u,dif.getObservations(0,0));
	ASSERT_EQ(40000u,dif.getObservations(1,0));

	Node& intel = n.getNode("Intelligence");
	ASSERT_EQ(70000u,intel.getObservations(0,0));
	ASSERT_EQ(30000u,intel.getObservations(1,0));

	Node& grade = n.getNode("Grade");
	ASSERT_EQ(13500u,grade.getObservations(0,0));
	ASSERT_EQ(1250u,grade.getObservations(0,2));
	ASSERT_EQ(13500u,grade.getObservations(0,1));
	ASSERT_EQ(7500u,grade.getObservations(0,3));
	ASSERT_EQ(18000u,grade.getObservations(1,0));
	ASSERT_EQ(6250u,grade.getObservations(1,2));
	ASSERT_EQ(1200u,grade.getObservations(1,1));
	ASSERT_EQ(4500u,grade.getObservations(1,3));
	ASSERT_EQ(13500u,grade.getObservations(2,0));
	ASSERT_EQ(17500u,grade.getObservations(2,2));
	ASSERT_EQ(300u,grade.getObservations(2,1));
	ASSERT_EQ(3000u,grade.getObservations(2,3));

	Node& SAT = n.getNode("SAT");
	ASSERT_EQ(66500u,SAT.getObservations(0,0));
	ASSERT_EQ(6000u,SAT.getObservations(0,1));
	ASSERT_EQ(3500u,SAT.getObservations(1,0));
	ASSERT_EQ(24000u,SAT.getObservations(1,1));
	
	Node& Letter = n.getNode("Letter");
	ASSERT_EQ(3575u , Letter.getObservations(0,0));
	ASSERT_EQ(11980u , Letter.getObservations(0,1));
	ASSERT_EQ(33957u , Letter.getObservations(0,2));
	ASSERT_EQ(32175u , Letter.getObservations(1,0));
	ASSERT_EQ(17970u , Letter.getObservations(1,1));
	ASSERT_EQ(343u , Letter.getObservations(1,2));
}

TEST_F(DataDistributionTest, distributeObservationsTestStudentInComplet){
	Network n;
	n.readNetwork(TEST_DATA_PATH("Student.na"));
	n.readNetwork(TEST_DATA_PATH("Student.sif"));
	Matrix<std::string> originalObservations (TEST_DATA_PATH("dataStudent60.txt"),false,true);
	Matrix<int> observations (0,0,-1);
	Discretiser disc(originalObservations,TEST_DATA_PATH("controlStudent.txt"), observations, n);
	DataDistribution db (n, observations);
	db.assignObservationsToNodes();
	
	db.distributeObservations();
	Node dif = n.getNode("Difficulty");
	ASSERT_EQ(59999u,dif.getObservations(0,0));
	
	ASSERT_EQ(23973u,dif.getObservations(1,0));
	ASSERT_EQ(16028u,dif.getObservations(2,0));

	Node& intel = n.getNode("Intelligence");
	ASSERT_EQ(59999u,intel.getObservations(0,0));
	
	ASSERT_EQ(27829u,intel.getObservations(1,0));
	ASSERT_EQ(12172u,intel.getObservations(2,0));

	Node& grade = n.getNode("Grade");
	ASSERT_EQ(4234u,grade.getObservations(0,0));
	ASSERT_EQ(1461u,grade.getObservations(0,1));
	ASSERT_EQ(2395u,grade.getObservations(0,2));
	ASSERT_EQ(1444u,grade.getObservations(0,3));
	
	ASSERT_EQ(914u,grade.getObservations(1,0));
	ASSERT_EQ(94u,grade.getObservations(1,2));
	ASSERT_EQ(924u,grade.getObservations(1,1));
	ASSERT_EQ(465u,grade.getObservations(1,3));
	ASSERT_EQ(1136u,grade.getObservations(2,0));
	ASSERT_EQ(84u,grade.getObservations(2,1));
	ASSERT_EQ(399u,grade.getObservations(2,2));
	ASSERT_EQ(304u,grade.getObservations(2,3));
	ASSERT_EQ(840u,grade.getObservations(3,0));
	ASSERT_EQ(12u,grade.getObservations(3,1));
	ASSERT_EQ(1081u,grade.getObservations(3,2));
	ASSERT_EQ(205u,grade.getObservations(3,3));

	Node& SAT = n.getNode("SAT");
	ASSERT_EQ(16641u,SAT.getObservations(0,0));
	ASSERT_EQ(7332u,SAT.getObservations(0,1));

	ASSERT_EQ(10601u,SAT.getObservations(1,0));
	ASSERT_EQ(1018u,SAT.getObservations(1,1));
	ASSERT_EQ(587u,SAT.getObservations(2,0));
	ASSERT_EQ(3822u,SAT.getObservations(2,1));
	
	Node& Letter = n.getNode("Letter");
	ASSERT_EQ(8570u , Letter.getObservations(0,0));
	ASSERT_EQ(7136u , Letter.getObservations(0,1));
	ASSERT_EQ(8266u , Letter.getObservations(0,2));
	
	ASSERT_EQ(547u , Letter.getObservations(1,0));
	ASSERT_EQ(1856u , Letter.getObservations(1,1));
	ASSERT_EQ(5494u , Letter.getObservations(1,2));
	ASSERT_EQ(5206u , Letter.getObservations(2,0));
	ASSERT_EQ(2875u , Letter.getObservations(2,1));
	ASSERT_EQ(51u, Letter.getObservations(2,2));
}
