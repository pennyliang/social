#include "thuirdb.h"
#include <iostream>
#include "string.h"
#include <stdlib.h>
#include "stdint.h"
#include <fstream>
#include <pthread.h>

using namespace std;

enum{COMMAND=1,KEY_FILE,DATA_FILE,FILECNT,FILE1};
struct thread_param
{
        thuirdb* m_db_instance;
        char m_file_name[256];
};
void* do_query(void* param)
{
	thread_param* p = (thread_param*)param;
	char* line_buffer = NULL;
	FILE *fp ;
	fp = fopen(p->m_file_name, "r");
	if( NULL == fp )
	{
		return 0;
	}
	ssize_t read = 0;
	bool quit = false;
	size_t len = 0;
	char* value = NULL;
	char* buffer = (char*)malloc(sizeof(char)*4096*2);
	uint32_t value_size = 0;
	while( (read = getline(&line_buffer, &len, fp)) != -1 && !quit )
	{
		line_buffer[read-1] = '\0';
		if(thuir_db_query_one_concurrent(p->m_db_instance,line_buffer,strlen(line_buffer),value,value_size))
		{
			if( value_size !=0 && value_size<=4096)
			{
				memcpy(buffer,value,value_size);
				buffer[value_size]='\0';
				printf("%s\t%s\n",line_buffer,buffer);
			}
		}

	}
	free(buffer);
	free(line_buffer);
	fclose(fp);
};

int main(int argc,char* argv[])
{
        if( 0 == strcmp(argv[COMMAND],"query"))
        {
		if(NULL == argv[KEY_FILE] || NULL == argv[DATA_FILE])
		{
			cout<<"parameter error!!"<<endl;
			return 0;
		}

                thuirdb* db = thuir_db_new();
                bool ret = thuir_db_r_open(db,argv[KEY_FILE],argv[DATA_FILE],true);
                if(!ret)
		{
			return 0;
		}
		int thread_cnt = atoi(argv[FILECNT]);
		pthread_t* thread = (pthread_t*) malloc(thread_cnt*sizeof( pthread_t));
	        thread_param* param_arr = (thread_param*) malloc(thread_cnt*sizeof(thread_param));
		for(int i = 0;i<thread_cnt;++i)
		{
			param_arr[i].m_db_instance = db;
			if(strlen(argv[FILE1+i])>255) return 0;
			strncpy(param_arr[i].m_file_name,argv[FILE1+i],strlen(argv[FILE1+i]));	
			pthread_create(&thread[i],NULL,do_query,&param_arr[i]); 
		}

		for(int i=0;i<thread_cnt;++i)
		{
        		pthread_join(thread[i],NULL);  
		}	
		free(thread);
		free(param_arr);
                thuir_db_quit(db);
        }
}
