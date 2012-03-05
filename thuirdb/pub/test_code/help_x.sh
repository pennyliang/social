
###### make #####################################################
make                                                            #
#################################################################

###### generate copus############################################
#if u don't have google corpus,just generate a random corpus    #
#10000:			cnt of corpus(wc -l)		       	#
echo "**********corpus generating start************"            #

record_cnt=10000000
record_cnt_per_thread=$(($record_cnt/10))


./bin/generate_key_value $record_cnt_per_thread > xaa &
./bin/generate_key_value $record_cnt_per_thread > xab & 
./bin/generate_key_value $record_cnt_per_thread > xac &
./bin/generate_key_value $record_cnt_per_thread > xad &
./bin/generate_key_value $record_cnt_per_thread > xae &
./bin/generate_key_value $record_cnt_per_thread > xaf &
./bin/generate_key_value $record_cnt_per_thread > xag &
./bin/generate_key_value $record_cnt_per_thread > xah &
./bin/generate_key_value $record_cnt_per_thread > xai &
./bin/generate_key_value $record_cnt_per_thread > xaj 
sleep 5m

echo "*********************************************"            #
#################################################################

#split corpus_file -l 10000000

##########################################################################################
#corpus_file: 		corpus file,such as google ngram                                 #
#./kvimp_file:  	used for import,sorted allready                                  #
#thuirdb.indexfile:     index file                                                       #
##thuirdb.datafile:     data file                                                        #
##########################################################################################
echo "***********preprocess start******************"
./bin/test_thuirdb multi-import_prprocess xaa ./data/kvimp_file1 ./db/thuirdb.indexfile ./db/thuirdb.datafile $record_cnt &
./bin/test_thuirdb multi-import_prprocess xab ./data/kvimp_file2 ./db/thuirdb.indexfile ./db/thuirdb.datafile $record_cnt &
./bin/test_thuirdb multi-import_prprocess xac ./data/kvimp_file3 ./db/thuirdb.indexfile ./db/thuirdb.datafile $record_cnt &
./bin/test_thuirdb multi-import_prprocess xad ./data/kvimp_file4 ./db/thuirdb.indexfile ./db/thuirdb.datafile $record_cnt &

./bin/test_thuirdb multi-import_prprocess xae ./data/kvimp_file5 ./db/thuirdb.indexfile ./db/thuirdb.datafile $record_cnt &
./bin/test_thuirdb multi-import_prprocess xaf ./data/kvimp_file6 ./db/thuirdb.indexfile ./db/thuirdb.datafile $record_cnt &
./bin/test_thuirdb multi-import_prprocess xag ./data/kvimp_file7 ./db/thuirdb.indexfile ./db/thuirdb.datafile $record_cnt &
./bin/test_thuirdb multi-import_prprocess xah ./data/kvimp_file8 ./db/thuirdb.indexfile ./db/thuirdb.datafile $record_cnt &

./bin/test_thuirdb multi-import_prprocess xai ./data/kvimp_file9 ./db/thuirdb.indexfile ./db/thuirdb.datafile $record_cnt  
./bin/test_thuirdb multi-import_prprocess xaj ./data/kvimp_file0 ./db/thuirdb.indexfile ./db/thuirdb.datafile $record_cnt 


echo "*********************************************"

sort -m ./data/kvimp* -n > ./kvimp_file

rm -rf ./data/kvimp*

./bin/test_thuirdb  build-dbfile ./kvimp_file $record_cnt ./db/thuirdb.indexfile ./db/thuirdb.datafile

##########################################################################################
#import:		command                                                          #
#thuirdb.indexfile:	index file                                                       #
#thuirdb.datafile:	data file                                                        #
#kvimp_file:	import file,must the same as the file previously get                     #
##########################################################################################
echo "***********import start**********************"
time nice -19 ./bin/test_thuirdb import ./db/thuirdb.indexfile ./db/thuirdb.datafile ./kvimp_file
echo "*********************************************"



##########################################################################################
#thuirdb.indexfile:     index file                                                       #
#thuirdb.datafile:      data file                                                        #
##"penny":       key                                                                     #
##########################################################################################
echo "***********query once************************"
time nice -19 ./bin/test_thuirdb query-one ./db/thuirdb.indexfile ./db/thuirdb.datafile thuirdb
echo "*********************************************"


##########################################################################################
#thuirdb.indexfile:     index file                                                       #
#thuirdb.datafile:      data file                                                        #
##"penny":       key                                                                     #
##########################################################################################
#echo "***********query all*************************"
#time nice -19 ./bin/test_thuirdb query-all ./db/thuirdb.indexfile ./db/thuirdb.datafile thuirdb
#echo "*********************************************"

##########################################################################################
#thuirdb.indexfile:     index file                                                       #
#thuirdb.datafile:      data file                                                        #
#test_key:              key file,                                                        #
##########################################################################################
echo "***********query start***********************"
head -10000 xaa | cut -f1  > ./data/test_key
time nice -19 ./bin/test_thuirdb query ./db/thuirdb.indexfile ./db/thuirdb.datafile ./data/test_key > ./data/test_key.value
#diff ./data/test_key.value corpus_file
echo "*********************************************"

##########################################################################################
#thuirdb.datafile:      data file                                                        #
##########################################################################################
echo "***********scan start************************"
#time nice -19 ./bin/test_thuirdb scan ./db/thuirdb.datafile > ./data/corpus_file.scaned
#sort corpus_file > ./data/s.corpus_file
#sort ./data/corpus_file.scaned > ./data/s.corpus_file.scaned
#diff ./data/s.corpus_file ./data/s.corpus_file.scaned
echo "*********************************************"


			
