#include "gtest/gtest.h"
#include "../NetworkController.h"
#include "../Parser.h"

class QueryTest : public ::testing::Test{
	protected:
	QueryTest()
		:c(NetworkController())
	{
	}

	void virtual SetUp(){
		c.loadNetwork("TestA.na");
		c.loadNetwork("TestSif.sif");
		c.loadObservations("data.txt","control.txt");
		c.trainNetwork();
	}

	public:
	NetworkController c;
	
};
/*
TEST_F(QueryTest,ParserCheck0){
	std::string query("");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck1){
	std::string query("This should throw");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck2){
	std::string query("? Grade g1");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck3){
	std::string query("? Grad = g1");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck4){
	std::string query("? Grade = g");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck5){
	std::string query("? Grade = g1 !");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck6){
	std::string query("? Grade = g1 do Letter = l0");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck7){
	std::string query("? Grade = g1 ! Letter = l0");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck8){
	std::string query("? Grade = g1 ! do Letter l0");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck9){
	std::string query("? Grade = g1 ! do Lette = l0");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck10){
	std::string query("? Grade = g1 ! do Letter = l");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck11){
	std::string query("? Grade = g1 +");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck12){
	std::string query("? Grade = g1 ! +");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck13){
	std::string query("? Grade = g1 ! + Letter");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck14){
	std::string query("? Grade = g1 ! + Letter Grad");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck15){
	std::string query("? Grade = g1 ! + Lette Grade");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck16){
	std::string query("? Grade = g1 ! + Lette Grad");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck17){
	std::string query("? Grade = g1 -");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck18){
	std::string query("? Grade = g1 ! -");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck19){

	std::string query("? Grade = g1 ! - Letter");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck20){
	std::string query("? Grade = g1 ! - Letter Grad");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck21){
	std::string query("? Grade = g1 ! - Lette Grade");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck22){
	std::string query("? Grade = g1 ! - Lette Grad");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck23){
	std::string query("? Grade = g1 | ");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck24){
	std::string query("? Grade = g1 | Letter l1 ");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck25){
	std::string query("? Grade = g1 | Letter = ");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck26){
	std::string query("? Grade = g1 | Lette = l1 ");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck27){
	std::string query("? Grade = g1 Lette");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck28){
	std::string query("? Grade = g1 Letter = l ");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck29){
	std::string query("? Grade = g1 Letter l1");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}
*/
TEST_F(QueryTest,ParserCheck30){
	std::string query("? argmax");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck31){
	std::string query("? argmax Letter");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck32){
	std::string query("? argmax ( Letter");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck33){
	std::string query("? argmax Letter )");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck34){
	std::string query("? argmax( Letter )");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest,ParserCheck35){
	std::string query("? argmax ( Letter)");
	Parser p (query,c);
	ASSERT_THROW(p.parseQuery(),std::invalid_argument);
}

TEST_F(QueryTest, ParserCheck36){
	std::string query("? Grade = g1");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.362f,qe.execute().first,0.001);
}

TEST_F(QueryTest, ParserCheck37){
	std::string query("? Grade = g2");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	qe.execute();
	ASSERT_NEAR(0.2884f,qe.execute().first,0.001);
}

TEST_F(QueryTest, ParserCheck38){
	std::string query("? Grade = g3");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	qe.execute();
	ASSERT_NEAR(0.3496f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck39){
	std::string query("? Intelligence = i0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	qe.execute();
	ASSERT_NEAR(0.7f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck40){
	std::string query("? Intelligence = i1");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	qe.execute();
	ASSERT_NEAR(0.3f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck41){
	std::string query("? Difficulty = d0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	qe.execute();
	ASSERT_NEAR(0.6f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck42){
	std::string query("? Difficulty = d1");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	qe.execute();
	ASSERT_NEAR(0.4f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck43){
	std::string query("? SAT = s0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	qe.execute();
	ASSERT_NEAR(0.725f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck44){
	std::string query("? SAT = s1");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.275f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck45){
	std::string query("? Letter = l0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.497664f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck46){
	std::string query("? Letter = l1");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.502336f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck47){
	std::string query("? Grade = g1 Difficulty = d0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.288f,qe.execute().first,0.001);
}

TEST_F(QueryTest, ParserCheck48){   
	std::string query("? Grade = g1 Difficulty = d0 Intelligence = i0 SAT = s0 Letter = l0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.01197f,qe.execute().first,0.001);
}

TEST_F(QueryTest, ParserCheck49){
	std::string query("? Grade = g1 | Intelligence = i0 Difficulty = d0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.3f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck50){
	std::string query("? Grade = g1 | Difficulty = d0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.48f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck51){
	std::string query("? Difficulty = d0 | SAT = s0");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.6f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck52){
	std::string query("? argmax ( Grade )");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.362f, qe.execute().first,0.001);
}

TEST_F(QueryTest, ParserCheck53){
	std::string query("? argmax ( SAT )");
	Parser p (query,c);
	QueryExecuter qe = p.parseQuery();
	ASSERT_NEAR(0.725f, qe.execute().first, 0.001);
}

TEST_F(QueryTest, ParserCheck54){
	std::string query("? argmax ( Intelligence )");
	Parser p (query, c);
	QueryExecuter qe = p.parseQuery();
	qe.execute();
	ASSERT_NEAR(0.7f, qe.execute().first, 0.001);	
}
