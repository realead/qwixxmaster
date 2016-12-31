

CC=g++ 
sources = $(wildcard src/*.cpp)
objects = $(sources:.cpp=.o)

PG=

all: $(sources) create

retest: clean test

rerun: clean run

rebuild: clean create

clean :
	rm -f src/*.o

%.o : %.cpp
	$(CC) -g $(PG) -std=c++11 -O2 -c $< -o $> $@

create : $(objects)
	$(CC) -g $(PG) $(objects) -o bin/quixxmaster
	
	
test: create
	python test/test_io.py test/cases

long_test: test
	python test/test_io.py test/long_runners

run: create
	bin/quixxmaster
	
testtime: create
	/usr/bin/time bin/quixxmaster -s 0 < test/time_cases/small.in
	/usr/bin/time bin/quixxmaster -s 0 < test/time_cases/medium.in
	/usr/bin/time bin/quixxmaster -s 10 < test/time_cases/long.in
