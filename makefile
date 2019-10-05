all: client server
client: client.o
	gcc client.c -o client -pthread
client.o: client.c
	gcc -g client.c -c
server: server.o
	gcc server.c -o server -pthread
server.o: server.c
	gcc -g server.c -c
clear:
	rm -rf *.o
