#define DEBUG
#ifdef DEBUG
 #define D if(1)
#else
 #define D if(0)
#endif

#define TOTALPEOPLE 10 //Total number of people that can be served by restaurant before shutting down
#define MAXQUEUE 5 //Maximum number of people in queue.
#define PERMS 0600 //Permission levels of shared memory
#define SHMKEY (key_t)6666 //Key used to create SHM
#define SEM_PERMS 0644 //Permission levels of semaphores.
#define STRLEN 1024
#include <semaphore.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h> //atoi and other library functions
#include <algorithm>

/* Searches for ID in clientData array. Returns nullptr if not found */
struct clientData * getClientById(int Id, struct clientData* shmdata);

/* Searches for itemID from our comma seperated menu. */
/* Format: itemID,Description,Price,Min_time,Maxtime */
char* findDescriptionMenu(int Id, std::vector<struct item> &menuList);
float findPriceMenu(int Id, std::vector<struct item> &menuList);
int findMin_timeMenu(int Id, std::vector<struct item> &menuList);
int findMax_timeMenu(int Id, std::vector<struct item> &menuList);
void loadMenu(const char* filename, std::vector<struct item> &menuList);
static struct item createStruct(char* itemLine);

/* Information about the client  */
struct clientData{
  int pid;
  int itemId;
  char description[STRLEN];
  float money_spent;
  int time_in_shop;
  int time_cashier_waiting;
  int time_food_waiting;
  int time_server_waiting;
  sem_t paid_sem; //Cashier will signal when client has paid.
};

/* The main shared memory struct for all the processes */
struct sharedData{
  sem_t lock_sem; //Initialized to 1. Lock the sharedData ensuring only one process can use shared.
  sem_t cashier_lock_sem; //Initialized to 1. Lock the cashier so only one cashier is operating

  sem_t coordinator_sem; //Initialized to 0. Signal by last client.

  sem_t total_cashier_queue_sem; //Initialized to 0. Incremented by clients entering queue. Decremented by cashier.
  sem_t cashier_queue_sem; //Initialized to 0. Decremented by clients in queue. Incremented by cashier when ready.
  sem_t cashier_signal; //Initialized to 0. Cashier waits for the chosen client to finish submitting clientpid.
  int clientpid; //clientpid will be put here awaiting for cashier to retrieve.

  sem_t total_server_queue_sem; //Initialized to 0. Incremented by clients entering queue. Decremented by server.
  sem_t server_queue_sem; //Initialized to 0. Decremented by clients in queue. Incremented by server when ready.
  sem_t client_signal; //Initialized to 0. Client waits for server to signal on it.
  int serverClientPid; //serverClientPid will be put here awaiting for server to retrieve.

  int total_served; //Initialized to 0. Incremented by new clients when leaving. Last client checks if equal to TOTALPEOPLE
  int numclients; //Number of clients. If exceeded this, then we can close doors
  struct clientData clients[TOTALPEOPLE]; //Client info is found here
};

/* struct for item on the menu vector */
struct item{
  int itemId;
  char description[STRLEN];
  float price;
  int min_time;
  int max_time;
};
