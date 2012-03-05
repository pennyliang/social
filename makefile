
CC = g++

CFLAGS = -g

LDFLAGS=

LIBOBJECTS = -lpthread -lgtest

SRC = unit_all.cpp ./utility/bfilter.cpp

OBJ = $(SRC:.cpp=.o)

PROGRAMS = unit_all





all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS)

$(PROGRAMS):$(OBJ)
	g++ -o $@ $(OBJ) $(LIBOBJECTS) $(LDFLAGS)

bfilter.o: ../utility/bfilter.h	../utility/bfilter.cpp
	g++ -c -g bfilter.cpp


