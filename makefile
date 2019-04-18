CFLAGS= -std=c++11

all: 	coordinator \
	client

coordinator: coordinator.o shared.o
	g++ $(CFLAGS) coordinator.o shared.o -o coordinator -lpthread

client: client.o shared.o
	g++ $(CFLAGS) client.o shared.o -o client -lpthread

client.o: client.cpp shared.h
	g++ $(CFLAGS) client.cpp -o client.o -c

coordinator.o: coordinator.cpp shared.h
	g++ $(CFLAGS) coordinator.cpp -o coordinator.o -c

shared.o: shared.cpp shared.h
	g++ $(CFLAGS) shared.cpp -o shared.o -c

clean:
	rm *.o coordinator client
