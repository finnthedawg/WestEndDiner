# 🍆 WestEndDiner 🍆
Simulating interprocess communication, scheduling and job processing in a restaurant setting.

---

West End Diner is a simulation of a restaurant using independent `client`, `cashier`, and `server` processes. It does this through the use of shared memory and semaphore IPCs which synchronizes and passes information across the independent participating programs.

---

#### Specifications of the restaurant:
* Clients processes arrive at random times and enter the cashier's queue
* The queue has a maximum size of `MAXQUEUE`. Clients cannot enter if the queue has reached `MAXQUEUE`.
* There can be any number of Cashiers who accept orders when they are free.
* If there are no people in the cashier queue then take a break between 1 and `breakTime`.
* Cashiers service the client between 1 and `serviceTime` during which they cannot service another client.
* Clients wait while their food is being prepared as specified in the menu.
* After the food is prepared, the client waits to be seated by the served which takes between 1 and serverTime.
* After eating the food for some time, the client can safely leave the restaurant.
* After the last client in the restaurant leaves, the restaurant can print summary statistics such as:

```
Average waiting in cashier queue 2.80 seconds
Average waiting for food 14.00 seconds
Average in server queue 0.50 seconds

Served a total of 10 clients generating $61.30 in revenue

//Top items
0. id 8, Club-Sandwich, ordered 3 times generating: 19.05
1. id 19, Mocha, ordered 2 times generating: 6.50
2. id 7, Mongolian-BBQ-Plate, ordered 2 times generating: 19.50
3. id 3, Garden-Salad, ordered 2 times generating: 9.50
4. id 5, Ceasars-Salad, ordered 1 times generating: 6.75
```

---

#### To build the project:

```
$ git clone
$ make
```
Cleaning object and build files:
```
$ make clean
```
Run the coordinator to initialize all shared memory and semaphores:
```
$ ./coordinator
```
The coordinator will output a `shmid` shared memory ID. You can pass this using the flag -m to our cashier/client/servers. However, You can also execute the programs because the same `SHMKEY (key_t)` is used to acquire the shared memory segment across the programs.

To run the `Cashier` / `Client` / `Server`
```
$ ./cashier -s serviceTime -b breakTime e.g -s 5 -b 10
$ ./server -s serviceTime -b breakTime e.g -s 3 -b 3
$ ./client -i itemId -e eatingTime e.g -i 8 -e 2
```
You can change the following settings in `shared.h`
```
#define TOTALPEOPLE 10 //Total number of people that can be served by restaurant before shutting down
#define MAXQUEUE 5 //Maximum number of people in queue.
#define SHMKEY (key_t)6666 //Key used to create SHM
#define STRLEN 1024
```

The csv menu can also be changed in `menu.txt`

---
#### Implementation details

In order to implement the program, a flowchart was designed to break down project as seen below:

<p align="center">
  <img width="750"  src="./WestEndFlowchart.png">
</p>

---
