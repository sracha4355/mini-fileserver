FLAGS=-Wall -std=c++20
CC=g++

all: 
	make tests

tests: tokenizer_tests.cpp tokenizer.o
	${CC} ${FLAGS} -o tokenizerTests $^

tokenizer.o: tokenizer.h tokenizer.cpp
	${CC} ${FLAGS} -c $^


clean:
	rm tokenizerTests
	rm *.o
	rm *.gch
