#include <gtest/gtest.h> 
#include "bfilter.h" 

TEST(bfilter, basic) 
{ 
	char command_line[0x100];
        
        snprintf(command_line,0x100,"dd if=/dev/zero of=./test/bf0 bs=1k count=10");
        system(command_line);

	snprintf(command_line,0x100,"dd if=/dev/zero of=./test/bf1 bs=1k count=10");
        system(command_line);	

	snprintf(command_line,0x100,"dd if=/dev/zero of=./test/bf2 bs=1k count=10");
        system(command_line);

	snprintf(command_line,0x100,"dd if=/dev/zero of=./test/bf3 bs=1k count=10");
        system(command_line);

	bfilter bf;
	bf.init("./test/bf0","./test/bf1","./test/bf2","./test/bf3");
	
	EXPECT_TRUE(bf.touch(123));
	
	EXPECT_TRUE(bf.is_touched(123));
	
	EXPECT_FALSE(bf.is_touched(456));
	
	EXPECT_TRUE(bf.touch(456));
	
	EXPECT_TRUE(bf.is_touched(456));

	EXPECT_TRUE(bf.is_touched(123));
}


TEST(bfilter, speical_bigint)
{
        char command_line[0x100];

        snprintf(command_line,0x100,"dd if=/dev/zero of=./test/bf0 bs=1k count=10");
        system(command_line);

        snprintf(command_line,0x100,"dd if=/dev/zero of=./test/bf1 bs=1k count=10");
        system(command_line);

        snprintf(command_line,0x100,"dd if=/dev/zero of=./test/bf2 bs=1k count=10");
        system(command_line);

        snprintf(command_line,0x100,"dd if=/dev/zero of=./test/bf3 bs=1k count=10");
        system(command_line);

        bfilter bf;
        bf.init("./test/bf0","./test/bf1","./test/bf2","./test/bf3");

        EXPECT_TRUE(bf.touch(0xFFFFFFFF));
	
        EXPECT_TRUE(bf.is_touched(0xFFFFFFFF));
	EXPECT_FALSE(bf.is_touched(0xFFFFFFFB));
}

