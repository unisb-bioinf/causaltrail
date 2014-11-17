#include "gtest/gtest.h"
#include "../Discretiser.h"
#include "../Network.h"

class DiscretiserTest : public ::testing::Test{
	protected:
	DiscretiserTest()
		:oobs_(Matrix<std::string>("testObservations.txt",false,true,"NA")), dobs_(Matrix<int>(0,0,0)), n_(Network())
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
}

TEST_F(DiscretiserTest,Ceil){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(1,0,0.0f);	
}

TEST_F(DiscretiserTest,Round){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(2,2,0.0f);	
}

TEST_F(DiscretiserTest,AMean){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(3,3,0.0f);	
}

TEST_F(DiscretiserTest,HMean){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(4,4,0.0f);	
}

TEST_F(DiscretiserTest,Median){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(5,5,0.0f);	
}

TEST_F(DiscretiserTest,Manually){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(6,6,3.0f);	
}

TEST_F(DiscretiserTest,BracketMedians){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(7,7,3.0);	
}

TEST_F(DiscretiserTest,PerasonTukey){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(8,8,0.0f);	
}

TEST_F(DiscretiserTest,MapNamesToInt){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(9,9,0.0f);	
}

TEST_F(DiscretiserTest,MapNamesToIntWithNA){
	Discretiser d = Discretiser(oobs_,dobs_,n_);
	d.discretiseRow(10,9,0.0f);	
}
