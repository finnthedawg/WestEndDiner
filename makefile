CFLAGS= -std=c++11

coordinator: coordinator.cpp
	g++ $(CFLAGS) coordinator.cpp -o coordinator

clean:
	rm *.o coordinator
