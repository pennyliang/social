
head -10000 corpus_file | cut -f1 > test_key

split -l 2500 test_key

echo "concurrently query start..."
time nice -19 ./bin/test_thuirdb_concurrent query ./db/thuirdb.indexfile ./db/thuirdb.datafile 4 xaa xab xac xad > test_key.result

sort test_key.result > s.test_key.result
head -10000 corpus_file | sort > s.corpus_file.1w

diff s.test_key.result s.corpus_file.1w

rm -rf xaa xab xac xad
rm -rf s.test_key.result s.corpus_file.1w
rm -rf  test_key.result test_key

