#include "bfilter.h"
bfilter::bfilter()
{
};
bfilter::~bfilter()
{
	sync();
}
bool bfilter::init(const char* filename1,const char* filename2,const char* filename3,const char* filename4)
{
	m_fd[0] = open(filename1, O_CREAT|O_RDWR|O_NONBLOCK,00777); if(m_fd[0] == -1) return false;
	m_fd[1] = open(filename1, O_CREAT|O_RDWR|O_NONBLOCK,00777); if(m_fd[1] == -1) return false;
	m_fd[2] = open(filename2, O_CREAT|O_RDWR|O_NONBLOCK,00777); if(m_fd[2] == -1) return false;
	m_fd[3] = open(filename3, O_CREAT|O_RDWR|O_NONBLOCK,00777); if(m_fd[3] == -1) return false;
	for(int i = 0;i<4;++i)
	{
		m_flen[i] = lseek(m_fd[i], 1, SEEK_END)-1; if(m_flen[i] == -1) return false;
		m_head[i] = (uint64_t*)mmap(0,m_flen[i],PROT_READ|PROT_WRITE,MAP_SHARED,m_fd[i],0);if(m_head[i] == MAP_FAILED) return false;
		m_max_idx[i] = m_flen[i]/8;
	}
	return true;
};
void bfilter::sync()
{
	for(int i=0;i<4;++i)
	{
		msync(m_head[i],m_flen[i],MS_ASYNC);
	}

};
bool bfilter::is_touched(uint32_t user_id)
{
	uint64_t md5_1 = raw_md5((char*)&user_id,4);

	char buf[16];
	snprintf(buf,16,"%d",user_id);
	uint64_t md5_2 = raw_md5(buf,strlen(buf));

	uint32_t seg[4];
	seg[0] = (((pldb_trick*)&md5_1)->v1);
	seg[1] = ((pldb_trick*)&md5_1)->v2;

	seg[2] = ((pldb_trick*)&md5_2)->v1;
	seg[3] = ((pldb_trick*)&md5_2)->v2;

	for(int i = 0;i<4;++i)
	{
		uint32_t idx1 = (seg[i]/64)%m_max_idx[i];
		uint32_t bdx1 =	(seg[i]%64);

		if(!test_bit(bdx1,&(m_head[i][idx1])))
		{
			return false;
		}
	}
	return true;

};
bool bfilter::touch(uint32_t user_id)
{
	uint64_t md5_1 = raw_md5((char*)&user_id,4);

	char buf[16];
	snprintf(buf,16,"%d",user_id);
	uint64_t md5_2 = raw_md5(buf,strlen(buf));

	uint32_t seg[4];
	seg[0] = (((pldb_trick*)&md5_1)->v1);
	seg[1] = ((pldb_trick*)&md5_1)->v2;

	seg[2] = ((pldb_trick*)&md5_2)->v1;
	seg[3] = ((pldb_trick*)&md5_2)->v2;

	for(int i = 0;i<4;++i)
	{
		uint32_t idx1 = (seg[i]/64)%m_max_idx[i];
		uint32_t bdx1 = (seg[i]%64);

		__set_bit(bdx1,&(m_head[i][idx1]));
	}
	return true;
};
