#include "thuirdb.h"
#include <iostream>
#include "string.h"
#include <stdlib.h>
#include "stdint.h"
#include <fstream>

using namespace std;
enum{COMMAND=1,KEY_FILE,DATA_FILE,KEY,VALUE,VALUE2,VALUE3};

enum{COPURS_FILE=2,KVIMP_FILE,KEYFILE,DATAFILE};

struct ngram_value
{
	char info[0];
};
void s_t_func(const char* str,char** type,uint32_t* type_size)
{
	int ret = 0;
	int str_len = strlen("NULL");
	if( 1 != sscanf(str,"%s",(((ngram_value*)(*type))->info)))
	{
		return;
	}
	*type_size = sizeof(ngram_value)+strlen((((ngram_value*)(*type))->info))+1;
};

int t_s_func(const void* type, char** str)
{
	ngram_value* n_v = (ngram_value*)type;
	return  snprintf(*str,64,"%s",(n_v->info));
};

struct att_s
{
        uint32_t cnt;
        uint32_t att[0];
        uint32_t size_()
        {
                return sizeof(att_s)+cnt*sizeof(uint32_t);
        }
};

int t_output(const void* type, char**str)
{
	ngram_value* n_v = (ngram_value*)type;
	sprintf(*str,"%s",(n_v->info));
	cout<<*str<<endl;
};


int main(int argc,char* argv[])
{
	if( 0 == strcmp(argv[COMMAND],"import_prprocess") && DATAFILE + 1 == argc)
	{
                thuirdb* db = thuir_db_new();
		bool ret = thuir_db_import_preprocess(db,argv[COPURS_FILE],argv[KVIMP_FILE],argv[KEYFILE],argv[DATAFILE],0); 	
		thuir_db_quit(db);
	}
	if( 0 == strcmp(argv[COMMAND],"import") && KEY + 1 == argc)
	{
		uint64_t suc_inserted_cnt;
                uint64_t fail_inserted_cnt;
	
		thuirdb* db = thuir_db_new();	
		bool ret = thuir_db_import(db,argv[KEY],argv[KEY_FILE],argv[DATA_FILE],suc_inserted_cnt,fail_inserted_cnt,s_t_func,0,'\t',true);
		if(ret)
		{
			cout<<"suc insert:"<<suc_inserted_cnt<<endl<<"fail inserted:"<<fail_inserted_cnt<<endl;
		}
		thuir_db_quit(db);	
	}
	
	if( 0 == strcmp(argv[COMMAND],"multi-import_prprocess") && DATAFILE + 2 == argc)
        {
                thuirdb* db = thuir_db_new();
		uint64_t corpus_file_cnt = strtoull(argv[DATAFILE+1],NULL,10);
                bool ret = thuir_db_import_preprocess(db,argv[COPURS_FILE],argv[KVIMP_FILE],argv[KEYFILE],argv[DATAFILE],corpus_file_cnt);
                thuir_db_quit(db);
        }
	if( 0 == strcmp(argv[COMMAND],"multi-import"))
        {
                uint64_t suc_inserted_cnt;
                uint64_t fail_inserted_cnt;

                thuirdb* db = thuir_db_new();
		uint64_t corpus_file_cnt = strtoull(argv[DATA_FILE+1],NULL,10);
                bool ret = thuir_db_import(db,argv[KEY],argv[KEY_FILE],argv[DATA_FILE],suc_inserted_cnt,fail_inserted_cnt,s_t_func,corpus_file_cnt,'\t',true);
                if(ret)
                {
                        cout<<"suc insert:"<<suc_inserted_cnt<<endl<<"fail inserted:"<<fail_inserted_cnt<<endl;
                }
                thuir_db_quit(db);
        }
	if( 0 == strcmp(argv[COMMAND],"build-dbfile"))
	{
		thuirdb* db = thuir_db_new();
                uint64_t corpus_file_cnt = strtoull(argv[3],NULL,10);
		thuir_db_build_dbfile(db, argv[2],corpus_file_cnt,argv[4],argv[5]);	
	}
        if( 0 == strcmp(argv[COMMAND],"query") && KEY + 1 == argc)
        {
		if(NULL == argv[KEY_FILE] || NULL == argv[DATA_FILE]|| NULL == argv[KEY])
		{
			cout<<"parameter error!!"<<endl;
			return 0;
		}

                thuirdb* db = thuir_db_new();
                bool ret = thuir_db_r_open(db,argv[KEY_FILE],argv[DATA_FILE],true);
                if(!ret )
		{
			return 0;
		}
		char* line_buffer = NULL;
		FILE *fp ;
		fp = fopen(argv[KEY], "r");
		if( NULL == fp )
		{
			return 0;
		}
		ssize_t read = 0;
		size_t  cnt = 0;
		bool quit = false;
		size_t len = 0;
		char* value = NULL;
                char* buffer = (char*)malloc(sizeof(char)*4096*2);
		uint32_t value_size = 0;
		while( (read = getline(&line_buffer, &len, fp)) != -1 && !quit )
		{
			line_buffer[read-1] = '\0';
			if(thuir_db_query_one(db,line_buffer,strlen(line_buffer),value,value_size))
			{
				if( value_size !=0 && value_size<=4096)
				{
					//cout<<line_buffer<<"\t"<<((ngram_value*)value)->v1<<" "<<((ngram_value*)value)->v2<<endl;
					printf("%s\t%s\n",line_buffer,((ngram_value*)value)->info);

				}
			}

		}		
		free(buffer);
		free(line_buffer);
		fclose(fp);
                thuir_db_quit(db);
        }
	
	if( 0 == strcmp(argv[COMMAND],"query-all") && KEY+1 == argc)
	{
		
                if(NULL == argv[KEY_FILE] || NULL == argv[DATA_FILE]|| NULL == argv[KEY])
                {
			cout<<"parameter error!!"<<endl;
                        return 0;
                }
		thuirdb* db = thuir_db_new();
                thuir_db_r_open(db,argv[KEY_FILE],argv[DATA_FILE],false);
		char* value = 0;
		char* buffer = (char*)malloc(sizeof(char)*4096*2);
		uint32_t value_size = 0;
		
		thuir_db_query_all(db,argv[KEY],strlen(argv[KEY]),t_output);
		
		free(buffer);
		thuir_db_quit(db);
	}

	if( 0 == strcmp(argv[COMMAND],"query-one") && KEY+1 == argc)
        {

                if(NULL == argv[KEY_FILE] || NULL == argv[DATA_FILE]|| NULL == argv[KEY])
                {
                        cout<<"parameter error!!"<<endl;
                        return 0;
                }
                thuirdb* db = thuir_db_new();
                thuir_db_r_open(db,argv[KEY_FILE],argv[DATA_FILE],false);
                char* value = 0;
                char* buffer = (char*)malloc(sizeof(char)*4096*2);
                uint32_t value_size = 0;

                if(thuir_db_query_one(db,argv[KEY],strlen(argv[KEY]),value,value_size))
                {
                        if(value_size != 0 && value_size<=4096*32)
                        {
                                printf("%s\t%s\n",argv[KEY],((ngram_value*)value)->info);
                        }
                }
                free(buffer);
                thuir_db_quit(db);
        }
	
	if( 0 == strcmp(argv[COMMAND],"scan"))
	{
		thuirdb* db = thuir_db_new();
		
		thuir_db_scan_all(db,argv[COMMAND+1],t_s_func);
	}
	return 0;
}
