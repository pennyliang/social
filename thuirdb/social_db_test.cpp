#include <gtest/gtest.h> 
#include "social_db.h" 

TEST(social, build_db) 
{ 
	EXPECT_TRUE(true);
}
TEST(social, build_index)
{
	EXPECT_TRUE(true);
}

#ifdef unit_test
int main(int argc, char** argv)
{
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}
#endif

