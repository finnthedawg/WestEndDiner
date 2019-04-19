#include <iostream>
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <unistd.h>          /* Parse flags, getpid() */
#include <stdlib.h>           /* rand() functions  */
#include <time.h>
#include <string.h>
#include <vector>

#include "shared.h"

using namespace std;
/* Options: ./client -i itemId -e eatTime -m shmid */
int main(int argc, char *argv[]) {

  /* Some default client values */
  int itemId = 9;
  int eatTime = 10;
  int shmid;

  /* Parse our arguments */
  int opt;
  while((opt = getopt(argc, argv, "i:e:m:")) != -1){
    switch(opt){
      case 'i':
        printf("ItemId set to %s\n", optarg);
        itemId = stoi(optarg);
        break;
      case 'e':
        printf("Max eatTime set to %s\n", optarg);
        eatTime = stoi(optarg);
        break;
      case 'm':
        printf("Shmid set to %s\n", optarg);
        shmid = stoi(optarg);
        break;
      case '?':
        printf("Unknown flag: %s\n", optarg);
        break;
    }
  }
  srand(time(0)); //randomize with time seed
  eatTime = rand() % eatTime;

  printf("I am client %d, ordering itemID %d, eatTime is %d\n", getpid(), itemId, eatTime);

  /* load the menu (Vector of items) */
  vector<struct item> menuList;
  loadMenu("menu.txt", menuList);
  float price = findPriceMenu(itemId,menuList);
  if(price == -1){
    perror("Unable to read menu\n");
    exit(1);
  }
  int min_time = findMin_timeMenu(itemId,menuList);
  int max_time = findMax_timeMenu(itemId,menuList);
  int cooking_time = (rand() % (max_time-min_time)) + min_time;
  char description[STRLEN];
  strcpy(description, findDescriptionMenu(itemId,menuList));
  printf("I finished reading the menu\n");
  printf("My order is %s, costs %f and will take %d seconds to cook.\n", description, price, cooking_time);

  /* Initialize our shared memory segment */
  struct sharedData *shmdata; //Our data struct stored in shared memory
  void *shmaddr; /* Pointer to head of shm*/

  /* Load our previous SHM associated with SHMKEY*/
  if ((shmid = shmget (SHMKEY, sizeof(struct sharedData), 0)) < 0){
    perror("Shmget could not create segment");
    exit(1);
  }
  D printf("Creating shared memory with SHMID: %d\n",shmid);

  /* Attaches the shared memory with id shmid*/
  if ((shmdata = (struct sharedData *)shmat(shmid, NULL, 0)) == (struct sharedData *)-1){
    perror("shmat could not attach");
    exit(1);
  }
  D printf("Attached shared memory with SHMID: %d\n",shmid);

  /*Before entering the queue, check the number of people */
  sem_wait(&shmdata -> lock_sem);
  int total_cashier_queue_sem;
  int total_server_queue_sem;
  if (sem_getvalue(&shmdata -> total_cashier_queue_sem, &total_cashier_queue_sem) == -1){
    perror("Could not get value of total_cashier_queue_sem");
    sem_post(&shmdata -> lock_sem);
    exit(1);
  }
  if (total_cashier_queue_sem >= MAXQUEUE){
    printf("Leaving because there are (%d) people in the queue. \n", total_cashier_queue_sem);
    sem_post(&shmdata -> lock_sem);
    exit(0);
  }
  /* Adds information to clients array */
  if (sem_init(&shmdata->clients[shmdata->numclients].paid_sem,1,0) == -1){
    perror("Failed to initialize semaphore of client in array");
    sem_post(&shmdata -> lock_sem);
    exit(-1);
  }
  shmdata->clients[shmdata->numclients].pid = getpid();
  shmdata->numclients ++;
  sem_post(&shmdata -> total_cashier_queue_sem);
  sem_post(&shmdata -> lock_sem);

  /*Get pointer to our client struct in the array*/
  struct clientData* thisClient = getClientById(getpid(), shmdata->clients);

  /*Wait in cashier_queue_sem until a cashier is ready */
  printf("Waiting in queue for cashier. Position(%d) \n", total_cashier_queue_sem+1);
  sem_wait(&shmdata -> cashier_queue_sem); //Cashier calls  us

  /* Submit out order to cashier */
  shmdata->clientpid = getpid();
  thisClient->itemId = itemId;
  strcpy(thisClient->description, description);
  thisClient->money_spent = price;
  sem_post(&shmdata -> cashier_signal);
  printf("Cashier called us, and we submitted order. Getting serviced...\n");

  sem_wait(&thisClient->paid_sem);
  printf("Cashier has serviced us and we paid our money. \n");

  /* Wait for food to be cooked */
  printf("Waiting for %s to be cooked... \n", description);
  sleep(cooking_time);
  printf("Food is ready... \n");

  /*Wait in server_queue_sem until a cashier is ready */
  if (sem_getvalue(&shmdata -> total_server_queue_sem, &total_server_queue_sem) == -1){
    perror("Could not get value of total_server_queue_sem");
    sem_post(&shmdata -> lock_sem);
    exit(1);
  }
  printf("Waiting to be seated by server. Position(%d) \n", total_server_queue_sem+1);
  sem_post(&shmdata -> total_server_queue_sem);
  sem_wait(&shmdata -> server_queue_sem);
  printf("Server called us. \n");
  sem_wait(&shmdata -> client_signal);
  printf("Server has finished seating us. \n");

  printf("Begin eating... \n");
  sleep(eatTime);
  printf("Finished eating! \n");
  /* Record time spent in shop */
  printf("Client has left the restaurant \n");

}
