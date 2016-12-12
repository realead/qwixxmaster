

CC=g++ 
sources = $(wildcard src/*.cpp)
objects = $(sources:.cpp=.o)

all: $(sources) create

retest: clean test

rerun: clean run

rebuild: clean create

clean :
	rm -f src/*.o

%.o : %.cpp
	$(CC) -g -std=c++11 -O2 -c $< -o $> $@

create : $(objects)
	$(CC) -g $(objects) -o bin/quixxmaster
	
	
test: create
	python test/test_io.py

run: create
	bin/quixxmaster
	
testtime: create
	/usr/bin/time bin/quixxmaster -s 10 < test/time_cases/small.in
	/usr/bin/time bin/quixxmaster -s 5 < test/time_cases/medium.in
	/usr/bin/time bin/quixxmaster -s 1 < test/time_cases/long.in
