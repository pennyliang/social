
###### make #####################################################
make                                                            #
#################################################################

###### generate copus############################################
#if u don't have google corpus,just generate a random corpus    #
#10000:			cnt of corpus(wc -l)		       	#
echo "**********corpus generating start************"            #
./bin/generate_key_value 10000 > corpus_file                    #
echo "*********************************************"            #
#################################################################


##########################################################################################
#corpus_file: 		corpus file,such as google ngram                                 #
#./kvimp_file:  	used for import,sorted allready                                  #
#thuirdb.indexfile:     index file                                                       #
##thuirdb.datafile:     data file                                                        #
##########################################################################################
echo "***********preprocess start******************"
time nice -19 ./bin/test_thuirdb import_prprocess corpus_file ./data/kvimp_file ./db/thuirdb.indexfile ./db/thuirdb.datafile
echo "*********************************************"


##########################################################################################
#import:		command                                                          #
#thuirdb.indexfile:	index file                                                       #
#thuirdb.datafile:	data file                                                        #
#kvimp_file:	import file,must the same as the file previously get                     #
##########################################################################################
echo "***********import start**********************"
time nice -19 ./bin/test_thuirdb import ./db/thuirdb.indexfile ./db/thuirdb.datafile ./data/kvimp_file
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
echo "***********query all*************************"
time nice -19 ./bin/test_thuirdb query-all ./db/thuirdb.indexfile ./db/thuirdb.datafile thuirdb
echo "*********************************************"

##########################################################################################
#thuirdb.indexfile:     index file                                                       #
#thuirdb.datafile:      data file                                                        #
#test_key:              key file,                                                        #
##########################################################################################
echo "***********query start***********************"
head -10000 corpus_file | cut -f1  > ./data/test_key
time nice -19 ./bin/test_thuirdb query ./db/thuirdb.indexfile ./db/thuirdb.datafile ./data/test_key > ./data/test_key.value
#diff ./data/test_key.value corpus_file
echo "*********************************************"

##########################################################################################
#thuirdb.datafile:      data file                                                        #
##########################################################################################
echo "***********scan start************************"
time nice -19 ./bin/test_thuirdb scan ./db/thuirdb.datafile > ./data/corpus_file.scaned
#sort corpus_file > ./data/s.corpus_file
#sort ./data/corpus_file.scaned > ./data/s.corpus_file.scaned
#diff ./data/s.corpus_file ./data/s.corpus_file.scaned
echo "*********************************************"


			
