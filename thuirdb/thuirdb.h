/* Copyright (C) 2000 thuir lab of Tsinghua University.
 * 
 * Introduction:The Thuir-db is designed mainly as a key-value data store engine, which have the same functions as most kv databases,such as  Berkely DB, TCB,Mongodb.
 * However, Thuir-db supports range-query,range-update,and range-delete,and inserting the same key,for it is based on the implemation of multiway search tree,
 * such as B+-tree,B*-tree.
 *
 * Constrain: The most siginfication constrain of Thuid-db is that it cann't support inserting keys after building, and the second constain of it is inserting must 
 * make sure that the key is ascending.
 *
 * Bug report: Please send the bugs to my email:mgigabyte#gmail.com,if you have any bugs on using.
 *
 * Coder:Pennyliang, http://blog.csdn.net/pennyliang/
 *
 */

#ifndef _THUIR_DB_H_
#define _THUIR_DB_H_

#include <stddef.h>
#include <stdint.h>

typedef void (*s_2_t_callback)(const char* str,char** type,uint32_t* type_size);

typedef int (*t_2_s_callback)(const void* type,char** str);


struct thuirdb;

#ifdef __cplusplus
extern "C"
{
#endif
/*DESCRIPTION:new thuirdb instance, on the environment of concurrency, please make sure only one instance created.
 *PARAM: none
 *RETURN VALUE: the instance of thuirdb	
 */ 
  thuirdb* thuir_db_new();


/*DESCRIPTION:open the thuirdb as the mode of read only,
 *PARAM:[db], the instance of a previously created thuirdb.
 *      [keyfile], the file name of index file
 *      [datafile],the file name of data file
 *      [in_memory], if true then all the index file will pull into the memory, and locked until the instance is quit.
 *RETURN VALUE: return true, if operation of open successed, otherwise return false.
 */
  bool thuir_db_r_open(thuirdb*& db,char* keyfile,char* datafile,bool in_memory=false);




  /*DESCRIPTION:query the value of the key
   *PARAM:[db],the instance of a previously imported thuirdb.
   *      [key],the address of key
   *      [keysize],the size of key
   *RETURN VALUE: return true,if the operation get the value rightly, and [value_addr] point to the address of value, [value_size] is stored the size of value
   */
  bool thuir_db_query_all(thuirdb*& db,void* key, uint32_t keysize, t_2_s_callback f);

  /*DESCRIPTION:query the value of the key
   *PARAM:[db],the instance of a previously imported thuirdb.
   *      [key],the address of key
   *      [keysize],the size of key
   *RETURN VALUE: return true,if the operation get the value rightly, and [value_addr] point to the address of value, [value_size] is stored the size of value
   */
  bool thuir_db_query_one(thuirdb*& db,void* key, uint32_t keysize, char*& value_addr,uint32_t& value_size);  

  /*DESCRIPTION:query the value of the key
   *PARAM:[db],the instance of a previously imported thuirdb.
   *      [key],the address of key:
   *      [keysize],the size of key
   * RETURN VALUE: return true,if the operation get the value rightly, and [value_addr] point to the address of value, [value_size] is stored the size of value
   */
  bool thuir_db_query_one_concurrent(thuirdb*& db,void* key, uint32_t keysize, char*& value_addr,uint32_t& value_size);


 /*DESCRIPTION:
  *PARAM:[corpus_file]:file name of corpus,every line is a key-value pair with the '\t' seperated.
  *      [kvimp_file]:the file use to insert into thuirdb
  *      [keyfile]:the file name of index file
  *      [datafile]:the file name of data file
  *      [record_count_of_corpus_file]:if the line count of the corpus is 1M,then chose the value 1M, .if don't know the count of the corpus, pls set zero as default to  *      make thuirdb to count the number
  *      uery.if don't know the count of the corpus set zero to make thuirdb to count the number
  */
  bool thuir_db_import_preprocess(thuirdb*& db,char* corpus_file,char* kvimp_file,char* keyfile,char* datafile,uint64_t record_count_of_corpus_file=0);
  

/*DESCRIPTION:import the corpus contained key-value infomation to the db
 *PARAM:[db],the instance of a previously imported thuirdb.
 *      [import_file]:the file name of corpus with key-value information stored
 *      [keyfile]:the file name of index file
 *      [datafile]:the file name of data file
 *      [f]:make the string to type
 *      [record_count_of_corpus_file]:if the line count of the corpus is 1M,then chose the value 1M, .if don't know the count of the corpus, pls set zero as default to make thuirdb to count the number
 *      [seperator]: the seperator means the info to split key and value. such as [key]\t[value],then \t is the seperator.
 *	[is_truncate],if true the keyfile and the datafile truncate to the smallest size.
 *
 *RETURN VALUE:return true,if the operation runs to the end,because the size of key is no more than 512 and value no more than 4k, so some key-value is inserted successfuly,while others unsuccessfuly,[suc_inserted_cnt] represents the former,and[fail_inserted_cnt] represents the latter.
 */
  bool thuir_db_import(thuirdb*& db, char* import_file,char* keyfile, char* datafile,uint64_t& suc_inserted_cnt,uint64_t& fail_inserted_cnt,s_2_t_callback  f,uint32_t record_count_of_corpus_file = 0,char seperator ='\t',bool is_truncate = true);	

/*DESCRIPTION:scan the data file to recreate the corpus file
 *PARAM:[db]:the instance of a previously imported thuirdb.
 *	[datafile]:the file name of data file
 *	[f]:use to make type to string
 *RETURN VALUE:return true, if the opertaion runs to the end without err,otherwise some error occure.
 */	

  bool thuir_db_scan_all(thuirdb*& db, char* datafile,t_2_s_callback f);


/*DESCRIPTION:delete the instance of thuirdb
 * PARAM:[db],the instance of thuirdb
 * RETURN VALUE:return true under any situation.
 */
  bool thuir_db_quit(thuirdb* db);

  bool thuir_db_build_dbfile(thuirdb*& db, char* corpus_file,uint64_t cnt_of_corpus,char* keyfile, char* datafile);
#ifdef __cplusplus
}
#endif

#endif
