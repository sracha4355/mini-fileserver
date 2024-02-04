FLAGS=-Wall -std=c++20
CC=g++


all::
	make tests

tests: parser_tests.cpp parser.o
	${CC} ${FLAGS} -o pTests $^ 

parser.o: parser.h parser.cpp
	${CC} ${FLAGS} -c $^ 

clean:
	rm pTests
	rm *.gch
	rm *.o
