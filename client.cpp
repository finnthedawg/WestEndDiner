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
  int total_queue_sem;
  if (sem_getvalue(&shmdata -> total_queue_sem, &total_queue_sem) == -1){
    perror("Could not get value of semaphore");
    sem_post(&shmdata -> lock_sem);
    exit(1);
  }
  if (total_queue_sem >= MAXQUEUE){
    printf("Leaving because there are (%d) people in the queue. \n", total_queue_sem);
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
  sem_post(&shmdata -> total_queue_sem);
  sem_post(&shmdata -> lock_sem);

  /*Wait in cashier_queue_sem until a cashier is ready */
  printf("Waiting in queue for cashier. Position(%d) \n", total_queue_sem);
  sem_wait(&shmdata -> cashier_queue_sem); //Cashier calls  us
  shmdata->clientpid = getpid();
  sem_post(&shmdata -> cashier_signal);
  printf("Cashier called us, and we submitted order. Waiting to be serviced \n");
  sem_wait(&getClientById(getpid(), shmdata->clients)->paid_sem);
  printf("Cashier has serviced us. \n");

}
