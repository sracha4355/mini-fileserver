CC=g++
FLAGS=-Wall

all:
	make client
	make server

client: client.cpp
	${CC} -o client $^

serverr: server.cpp
	${CC} -o server $^

clean:
	rm server
	rm client

