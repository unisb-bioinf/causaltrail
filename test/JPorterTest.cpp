#include "gtest/gtest.h"
#include "../core/JPorter.h"
#include "config.h"

class JPorterTest : public ::testing::Test{
	protected:
	JPorterTest()
	{
	}

};

TEST_F(JPorterTest,Constructor){
	JPorter JP();
}

