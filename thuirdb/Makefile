# Copyright (c) 2011 The THUIRDB Authors. All rights reserved.
CC = g++

CFLAGS = -g

LDFLAGS=-lpthread -lssl

LIBOBJECTS = ./lib/libthuirdb.a ./lib/libcpldb.a

PROGRAMS = test_thuirdb test_thuirdb_concurrent generate_key_value


all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS)

test_thuirdb: 
	$(CC) -o ./bin/$@ $@.cpp $(LIBOBJECTS) $(LDFLAGS)

test_thuirdb_concurrent: 
	$(CC) -o ./bin/$@ $@.cpp $(LIBOBJECTS) $(LDFLAGS)

generate_key_value:
	$(CC) -o ./bin/$@ $@.cpp $(LDFLAGS)

