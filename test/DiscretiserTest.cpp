#include "gtest/gtest.h"
#include "../core/Discretiser.h"
#include "../core/Network.h"
#include "config.h"

class DiscretiserTest : public ::testing::Test{
	protected:
	DiscretiserTest()
		:oobs_(Matrix<std::string>(TEST_DATA_PATH("testObservations.txt"),false,true)), dobs_(Matrix<int>(0,0,0)), n_(Network())
	{
	}

	public:
	Matrix<std::string> oobs_;
	Matrix<int> dobs_;
	Network n_;
};

TEST_F(DiscretiserTest,Floor){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(0,1,0.0f);	
	ASSERT_EQ(0,d.getEntry(0,0));
	ASSERT_EQ(1,d.getEntry(1,0));
	ASSERT_EQ(1,d.getEntry(2,0));
	ASSERT_EQ(2,d.getEntry(3,0));
	ASSERT_EQ(3,d.getEntry(4,0));
	ASSERT_EQ(4,d.getEntry(5,0));
}

TEST_F(DiscretiserTest,Ceil){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(1,0,0.0f);	
	ASSERT_EQ(0,d.getEntry(0,1));
	ASSERT_EQ(1,d.getEntry(1,1));
	ASSERT_EQ(1,d.getEntry(2,1));
	ASSERT_EQ(2,d.getEntry(3,1));
	ASSERT_EQ(3,d.getEntry(4,1));
	ASSERT_EQ(4,d.getEntry(5,1));
}

TEST_F(DiscretiserTest,Round){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(2,2,0.0f);	
	ASSERT_EQ(0,d.getEntry(0,2));
	ASSERT_EQ(0,d.getEntry(1,2));
	ASSERT_EQ(0,d.getEntry(2,2));
	ASSERT_EQ(1,d.getEntry(3,2));
	ASSERT_EQ(2,d.getEntry(4,2));
	ASSERT_EQ(3,d.getEntry(5,2));
}

TEST_F(DiscretiserTest,AMean){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(3,3,0.0f);	
	ASSERT_EQ(0,d.getEntry(0,3));
	ASSERT_EQ(0,d.getEntry(1,3));
	ASSERT_EQ(0,d.getEntry(2,3));
	ASSERT_EQ(1,d.getEntry(3,3));
	ASSERT_EQ(1,d.getEntry(4,3));
	ASSERT_EQ(1,d.getEntry(5,3));
}

TEST_F(DiscretiserTest,HMean){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(4,4,0.0f);	
	ASSERT_EQ(0,d.getEntry(0,4));
	ASSERT_EQ(0,d.getEntry(1,4));
	ASSERT_EQ(0,d.getEntry(2,4));
	ASSERT_EQ(1,d.getEntry(3,4));
	ASSERT_EQ(1,d.getEntry(4,4));
	ASSERT_EQ(1,d.getEntry(5,4));
}

TEST_F(DiscretiserTest,Median){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(5,5,0.0f);	
	ASSERT_EQ(0,d.getEntry(0,5));
	ASSERT_EQ(0,d.getEntry(1,5));
	ASSERT_EQ(0,d.getEntry(2,5));
	ASSERT_EQ(1,d.getEntry(3,5));
	ASSERT_EQ(1,d.getEntry(4,5));
	ASSERT_EQ(1,d.getEntry(5,5));
}

TEST_F(DiscretiserTest,Manually){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(6,6,3.0f);	
	ASSERT_EQ(0,d.getEntry(0,6));
	ASSERT_EQ(0,d.getEntry(1,6));
	ASSERT_EQ(0,d.getEntry(2,6));
	ASSERT_EQ(1,d.getEntry(3,6));
	ASSERT_EQ(1,d.getEntry(4,6));
	ASSERT_EQ(1,d.getEntry(5,6));
}

TEST_F(DiscretiserTest,BracketMedians){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(7,7,3.0);	
	ASSERT_EQ(0,d.getEntry(0,7));
	ASSERT_EQ(0,d.getEntry(1,7));
	ASSERT_EQ(1,d.getEntry(2,7));
	ASSERT_EQ(1,d.getEntry(3,7));
	ASSERT_EQ(2,d.getEntry(4,7));
	ASSERT_EQ(2,d.getEntry(5,7));
}

TEST_F(DiscretiserTest,PerasonTukey){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(8,8,0.0f);	
	ASSERT_EQ(0,d.getEntry(0,8));
	ASSERT_EQ(1,d.getEntry(1,8));
	ASSERT_EQ(0,d.getEntry(2,8));
	ASSERT_EQ(1,d.getEntry(3,8));
	ASSERT_EQ(1,d.getEntry(4,8));
	ASSERT_EQ(2,d.getEntry(5,8));
}

TEST_F(DiscretiserTest,MapNamesToInt){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(9,9,0.0f);	
	ASSERT_EQ(1,d.getEntry(0,9));
	ASSERT_EQ(0,d.getEntry(1,9));
	ASSERT_EQ(1,d.getEntry(2,9));
	ASSERT_EQ(1,d.getEntry(3,9));
	ASSERT_EQ(0,d.getEntry(4,9));
	ASSERT_EQ(0,d.getEntry(5,9));
}

TEST_F(DiscretiserTest,MapNamesToIntWithNA){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(10,9,0.0f);	
	ASSERT_EQ(1,d.getEntry(0,10));
	ASSERT_EQ(-1,d.getEntry(1,10));
	ASSERT_EQ(-1,d.getEntry(2,10));
	ASSERT_EQ(-1,d.getEntry(3,10));
	ASSERT_EQ(-1,d.getEntry(4,10));
	ASSERT_EQ(0,d.getEntry(5,10));
}
