# 🍆 WestEndDiner 🍆
Simulating interprocess communication, scheduling and job processing in a restaurant setting.

---

West End Diner is a simulation of a restaurant using independent `client`, `cashier`, and `server` processes. It does this through the use of shared memory and semaphore IPCs which synchronizes and passes information across the independent participating programs.

---

Design of the restaurant:
* Clients processes arrive at random times and enter the cashier's queue
* The queue has a maximum size of `MAXQUEUE`. Clients cannot enter if the queue has reached `MAXQUEUE`.
* There can be any number of Cashiers who accept orders when they are free.
* If there are no people in the cashier queue then take a break between 1 and `breakTime`.
* Cashiers service the client between 1 and `serviceTime` during which they cannot service another client.
* Clients wait while their food is being prepared as specified in the menu.
* After the food is prepared, the client waits to be seated by the served which takes between 1 and serverTime.
* After eating the food for some time, the client can safely leave the restaurant.
* After the last client in the restaurant leaves, the restaurant can print summary statistics including:


<p align="center">
  <img width="750"  src="./WestEndFlowchart.png">
</p>
