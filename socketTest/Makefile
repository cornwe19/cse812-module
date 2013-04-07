socket_utils.o: socket_utils.cpp
	g++ -c -Wall socket_utils.cpp -o bin/socket_utils.o

send: send.cpp socket_utils.o
	g++ send.cpp bin/socket_utils.o -g -o bin/send

receive: receive.cpp socket_utils.o
	g++ receive.cpp bin/socket_utils.o -g -o bin/receive