#ifndef _BLOOM_FILTER_H_
#define _BLOOM_FILTER_H_
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "stdint.h"
#include <iostream>
#include "string.h"
#include "stdio.h"

static  void __set_bit(int nr, uint64_t* addr)
{
        addr[0] |= ( 1UL << nr);
};

static bool test_bit(int nr,uint64_t* addr)
{
        return (0 < (addr[0] & (1UL<<nr)));
}; 

static inline uint64_t raw_md5(const char *str, int str_len)
{
    uint64_t hash = 5381;

    for(int i=0;i<str_len;++i)
    {
        hash += (hash << 5) + (*str++);
    }

    return (hash & 0x7FFFFFFF);
};

struct pldb_trick
{
        uint32_t v1;
        uint32_t v2;
};

class bfilter
{
	public:
		bfilter();
		~bfilter();
		bool init(const char* filename1,const char* filename2,const char* filename3,const char* filename4);
		void sync();
		bool is_touched(uint32_t user_id);
		bool touch(uint32_t user_id);
	private:
		int   m_fd[4];
		off_t m_flen[4];
		uint64_t * m_head[4];
		uint64_t m_max_idx[4];	
};

#endif
