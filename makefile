CFLAGS= -std=c++11

all: 	coordinator \
	client \
	cashier \
	server

coordinator: coordinator.o shared.o
	g++ $(CFLAGS) coordinator.o shared.o -o coordinator -lpthread

client: client.o shared.o
	g++ $(CFLAGS) client.o shared.o -o client -lpthread

cashier: cashier.o shared.o
	g++ $(CFLAGS) cashier.o shared.o -o cashier -lpthread

server: server.o shared.o
	g++ $(CFLAGS) server.o shared.o -o server -lpthread

server.o: server.cpp shared.h
	g++ $(CFLAGS) server.cpp -o server.o -c

cashier.o: cashier.cpp shared.h
	g++ $(CFLAGS) cashier.cpp -o cashier.o -c

client.o: client.cpp shared.h
	g++ $(CFLAGS) client.cpp -o client.o -c

coordinator.o: coordinator.cpp shared.h
	g++ $(CFLAGS) coordinator.cpp -o coordinator.o -c

shared.o: shared.cpp shared.h
	g++ $(CFLAGS) shared.cpp -o shared.o -c

clean:
	rm *.o coordinator client cashier
