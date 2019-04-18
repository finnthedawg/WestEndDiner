CFLAGS= -std=c++11

coordinator: coordinator.o shared.o
	g++ $(CFLAGS) coordinator.o shared.o -o coordinator

coordinator.o: coordinator.cpp
	g++ $(CFLAGS) coordinator.cpp -o coordinator.o -c

shared.o: shared.cpp
	g++ $(CFLAGS) shared.cpp -o shared.o -c

clean:
	rm *.o coordinator
