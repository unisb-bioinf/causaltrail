#include "gtest/gtest.h"
#include "../core/NetworkController.h"
#include "../core/Parser.h"
#include "config.h"

class ParserTest : public ::testing::Test{
	protected:
	ParserTest()
		:c(NetworkController())	{
		c.loadNetwork(TEST_DATA_PATH("Student.na"));
		c.loadNetwork(TEST_DATA_PATH("Student.sif"));
//		c.loadObservations(TEST_DATA_PATH("StudentData.txt"),TEST_DATA_PATH("controlStudent.txt"));
//		c.trainNetwork();
	}

	public:
	NetworkController c;
	
};

TEST_F(ParserTest,ParserCheck0){
	std::string query("");
	ASSERT_THROW(Parser p (query,c),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck1){
	std::string query("This should throw");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck2){
	std::string query("? Grade g1");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck3){
	std::string query("? Grad = g1");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck4){
	std::string query("? Grade = g");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck5){
	std::string query("? Grade = g1 !");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck6){
	std::string query("? Grade = g1 do Letter = l0");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck7){
	std::string query("? Grade = g1 ! Letter = l0");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck8){
	std::string query("? Grade = g1 ! do Letter l0");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck9){
	std::string query("? Grade = g1 ! do Lette = l0");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck10){
	std::string query("? Grade = g1 ! do Letter = l");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck11){
	std::string query("? Grade = g1 +");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck12){
	std::string query("? Grade = g1 ! +");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck13){
	std::string query("? Grade = g1 ! + Letter");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck14){
	std::string query("? Grade = g1 ! + Letter Grad");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck15){
	std::string query("? Grade = g1 ! + Lette Grade");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck16){
	std::string query("? Grade = g1 ! + Lette Grad");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck17){
	std::string query("? Grade = g1 -");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck18){
	std::string query("? Grade = g1 ! -");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck19){

	std::string query("? Grade = g1 ! - Letter");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck20){
	std::string query("? Grade = g1 ! - Letter Grad");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck21){
	std::string query("? Grade = g1 ! - Lette Grade");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck22){
	std::string query("? Grade = g1 ! - Lette Grad");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck23){
	std::string query("? Grade = g1 | ");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck24){
	std::string query("? Grade = g1 | Letter l1 ");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck25){
	std::string query("? Grade = g1 | Letter = ");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck26){
	std::string query("? Grade = g1 | Lette = l1 ");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck27){
	std::string query("? Grade = g1 Lette");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck28){
	std::string query("? Grade = g1 Letter = l ");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck29){
	std::string query("? Grade = g1 Letter l1");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck30){
	std::string query("? argmax");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck31){
	std::string query("? argmax Letter");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck32){
	std::string query("? argmax ( Letter");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck33){
	std::string query("? argmax Letter )");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck34){
	std::string query("? argma( Letter )");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck35){
	std::string query(" argmax ( Letter)");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}


TEST_F(ParserTest, ParserCheck36){
	std::string query("? Grade = g1");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.362f,qe.execute().first,0.001);
}

TEST_F(ParserTest, ParserCheck37){
	std::string query("? Grade = g2");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.2884f,qe.execute().first,0.001);
}

TEST_F(ParserTest, ParserCheck38){
	std::string query("? Grade = g3");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.3496f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck39){
	std::string query("? Intelligence = i0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.7f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck40){
	std::string query("? Intelligence = i1");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.3f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck41){
	std::string query("? Difficulty = d0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.6f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck42){
	std::string query("? Difficulty = d1");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.4f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck43){
	std::string query("? SAT = s0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.725f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck44){
	std::string query("? SAT = s1");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.275f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck45){
	std::string query("? Letter = l0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.497664f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck46){
	std::string query("? Letter = l1");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.502336f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck47){
	std::string query("? Grade = g1 Difficulty = d0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.288f,qe.execute().first,0.001);
}

TEST_F(ParserTest, ParserCheck48){   
	std::string query("? Grade = g1 Difficulty = d0 Intelligence = i0 SAT = s0 Letter = l0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.01197f,qe.execute().first,0.001);
}

TEST_F(ParserTest, ParserCheck49){
	std::string query("? Grade = g1 | Intelligence = i0 Difficulty = d0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.3f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck50){
	std::string query("? Grade = g1 | Difficulty = d0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.48f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck51){
	std::string query("? Difficulty = d0 | SAT = s0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.6f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck55){
	std::string query("? Difficulty = d0 | Grade = g1");
	Parser p(query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.795f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck52){
	std::string query("? argmax ( Grade )");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.362f, qe.execute().first,0.001);
}

TEST_F(ParserTest, ParserCheck53){
	std::string query("? argmax ( SAT )");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.725f, qe.execute().first, 0.001);
}

TEST_F(ParserTest, ParserCheck54){
	std::string query("? argmax ( Intelligence )");
	Parser p (query, c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.7f, qe.execute().first, 0.001);	
}

TEST_F(ParserTest, ParserCheck56){
	std::string query("? argmax ( Grade ) | Intelligence = i1");
	Parser p (query, c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.74f, qe.execute().first, 0.001);	
}

TEST_F(ParserTest, ParserCheck57){
	std::string query("? Intelligence = i1 | Intelligence = i0");
	Parser p (query, c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_THROW(qe.execute(),std::invalid_argument);
}


TEST_F(ParserTest, ParserCheck58){
	std::string query("? Letter = l1 | Letter = l0 ! + SAT Letter");
	Parser p (query, c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_THROW(qe.execute(),std::invalid_argument);
}

TEST_F(ParserTest, ParserCheck59){
	std::string query("? Letter = l1 | Letter = l0");
	Parser p (query, c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.4016, qe.execute().first, 0.01);
}

TEST_F(ParserTest, ParserCheck60){
	std::string query("? Letter = l1 ! do Grade = g1 | Letter = l0 Grade = g3");
	Parser p (query, c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.90, qe.execute().first, 0.001);
}


TEST_F(ParserTest, ParserCheck61){
	std::string query("? Letter = l1 ! do Intelligence = i1 | Intelligence = i0");
	Parser p (query, c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_THROW(p.parseQuery().execute(),std::invalid_argument);
}

TEST_F(ParserTest,ParserCheck62){
	std::string query("? Letter = l1 ! + Letter Intelligence");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery().execute(),std::invalid_argument);
}
