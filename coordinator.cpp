#include <iostream>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <vector>

#include "shared.h"

using namespace std;

/* Prints the orders of customers in our database */
void printOrders(struct clientData clients[TOTALPEOPLE]);
/* Prints average waiting times for every customer */
void printAvgWaiting(struct clientData clients[TOTALPEOPLE]);
/* Prints the number of clients served and the total revenue */
void printClientsRevenue(struct clientData clients[TOTALPEOPLE]);
/* Prints the top N most popular items and revenue */
void printTopNItems(struct clientData clients[TOTALPEOPLE], int n);

int main(int argc, char *argv[]) {
  struct sharedData *shmdata; //Our data struct stored in shared memory
  int shmid;
  void *shmaddr; /* Pointer to head of shm*/

  /* Create SHM with our shared SHMKEY*/
  if ((shmid = shmget (SHMKEY, sizeof(struct sharedData), PERMS | IPC_CREAT)) < 0){
    perror("Shmget could not create segment");
    exit(1);
  }
  D printf("Creating shared memory with SHMID: %d\n",shmid);

  /* Attaches the shared memory with id shmid*/
  if ((shmaddr = shmat(shmid, NULL, 0)) == (char *)-1){
    perror("shmat could not attach");
    exit(1);
  }
  D printf("Attached shared memory with SHMID: %d\n",shmid);

  /* Convert shmaddr pointer to our data pointer. */
  shmdata = (struct sharedData *)shmaddr;

  /* Initialize semaphores in shared memory*/
  if (sem_init(&shmdata -> coordinator_sem,1,0) == -1){
    perror("Failed to initialize coordinator_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> total_cashier_queue_sem,1,0) == -1){
    perror("Failed to initialize total_cashier_queue_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> cashier_queue_sem,1,0) == -1){
    perror("Failed to initialize cashier_queue_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> cashier_signal,1,0) == -1){
    perror("Failed to initialize cashier_signal");
    exit(-1);
  }
  if (sem_init(&shmdata -> lock_sem,1,1) == -1){
    perror("Failed to initialize lock_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> cashier_lock_sem,1,1) == -1){
    perror("Failed to initialize cashier_lock_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> total_server_queue_sem,1,0) == -1){
    perror("Failed to initialize total_server_queue_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> server_queue_sem,1,0) == -1){
    perror("Failed to initialize server_queue_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> client_signal,1,0) == -1){
    perror("Failed to initialize client_signal");
    exit(-1);
  }
  D printf("Initialized all semaphores: %d\n",shmid);

  /* Zero out the client array.*/
  shmdata -> numclients = 0;
  shmdata -> total_served = 0;
  for (int i = 0; i < TOTALPEOPLE; i ++){
    shmdata->clients[i].pid = -1;
  }
  D printf("Initialized all clients. \n");
  printf("Completed initialization. shmid is: %d\n",shmid);
  cout << "Summary statistics will be printed after we serve (%d) customers" << endl;
  sem_wait(&shmdata -> coordinator_sem);

  /* After the last client has left, create summary information */
  printf("*****-------------------------------------------------*******\n");
  printf("\n");
  printOrders(shmdata->clients);
  printf("\n");
  printf("*****-------------------------------------------------*******\n");
  printf("\n");
  printAvgWaiting(shmdata->clients);
  printf("\n");
  printf("*****-------------------------------------------------*******\n");
  printf("\n");
  printClientsRevenue(shmdata->clients);
  printf("\n");
  printf("*****-------------------------------------------------*******\n");
  printf("\n");
  printTopNItems(shmdata->clients, 5);
  printf("\n");

  /*Mark SHM to be destroyed*/
  if (shmctl(shmid, IPC_RMID, 0) == -1){
    perror("shm could be removed");
    exit(1);
  }
  D printf("Marked memory with SHMID to be removed when last process detaches: %d\n",shmid);

  /* Detach our shared memory */
  if (shmdt(shmaddr) == -1){
    perror("shm could not detach");
    exit(1);
  }
  D printf("Detached shared memory with SHMID: %d\n",shmid);

}

void printOrders(struct clientData clients[TOTALPEOPLE]){
  for (int i = 0; i < TOTALPEOPLE; i++){
    if (clients[i].pid == -1){
      continue;
    }
    printf("Client ID:%d ",clients[i].pid);
    printf("Time in shop: %d(s) ", clients[i].time_in_shop);
    printf("Spent: $%.2f ", clients[i].money_spent);
    printf("In queue of cashier %d(s) cook %d(s) and server %d(s)\n", clients[i].time_cashier_waiting, clients[i].time_food_waiting, clients[i].time_server_waiting);
  }
}

void printAvgWaiting(struct clientData clients[TOTALPEOPLE]){
  int customers = 0;
  int TotalcashierWait = 0;
  int TotalfoodWait = 0;
  int TotalserverWait = 0;
  for (int i = 0; i < TOTALPEOPLE; i++){
    if (clients[i].pid == -1){
      continue;
    }
    customers ++;
    TotalcashierWait += clients[i].time_cashier_waiting;
    TotalfoodWait += clients[i].time_food_waiting;
    TotalserverWait += clients[i].time_server_waiting;
  }
  printf("Average waiting in cashier queue %.2f seconds\n", (float)TotalcashierWait/customers);
  printf("Average waiting for food %.2f seconds\n", (float)TotalfoodWait/customers);
  printf("Average in server queue %.2f seconds\n", (float)TotalserverWait/customers);
}

void printClientsRevenue(struct clientData clients[TOTALPEOPLE]){
  int customers = 0;
  float revenue = 0;
  for (int i = 0; i < TOTALPEOPLE; i++){
    if (clients[i].pid == -1){
      continue;
    }
    customers ++;
    revenue += clients[i].money_spent;
  }
  printf("Served a total of %d clients generating $%.2f in revenue\n", customers, revenue);
}

void printTopNItems(struct clientData clients[TOTALPEOPLE], int n){
  struct itemOrder{
    int itemId;
    char description[STRLEN];
    float totalSpent;
    int timesOrdered;
  };
  vector<struct itemOrder> orders;
  /* Populate the summary statistics */
  for (int i = 0; i < TOTALPEOPLE; i++){
    if (clients[i].pid == -1){
      continue;
    }
    /*Add or append counts for each order in orderArray */
    vector<struct itemOrder>::iterator it;
    it = find_if(orders.begin(), orders.end(), [&](struct itemOrder item){
      return item.itemId ==  clients[i].itemId;
    });
    /* Increment the values of existing struct or create it */
    if (it != orders.end()){
      (*it).totalSpent += clients[i].money_spent;
      (*it).timesOrdered++;
    } else {
      struct itemOrder newOrder;
      newOrder.itemId = clients[i].itemId;
      strcpy(newOrder.description, clients[i].description);
      newOrder.totalSpent = clients[i].money_spent;
      newOrder.timesOrdered = 1;
      orders.push_back(newOrder);
    }
  }

  /* Sort the vector based on the orderCount */
  sort(orders.begin(), orders.end(), [](struct itemOrder item1, struct itemOrder item2){
    return item1.timesOrdered > item2.timesOrdered;
  });

  /* Print the top 5 */
  for (int i = 0; i < n && i < orders.size(); i++){
    printf("%d. id %d, %s, ordered %d times generating: %.2f\n",
    i, orders[i].itemId, orders[i].description, orders[i].timesOrdered, orders[i].totalSpent);
  }
}
