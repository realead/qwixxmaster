

CC=g++ 
sources = $(wildcard src/*.cpp)
objects = $(sources:.cpp=.o)

all: $(sources) create

clean :
	rm -f src/*.o

%.o : %.cpp
	$(CC) -std=c++11 -O2 -c $< -o $> $@

create : $(objects)
	$(CC) $(objects) -o bin/quixxmaster
	
	
test: create
	python test/test_io.py

run: create
	bin/quixxmaster
