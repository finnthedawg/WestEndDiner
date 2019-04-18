#include <iostream>
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <unistd.h>          /* Parse flags */
#include <stdlib.h>           /* rand functions  */
#include <time.h>

#include "shared.h"

using namespace std;
/* Options: ./client -i itemId -e eatTime -m shmid */
int main(int argc, char *argv[]) {

  /* Some default client values */
  int itemId = 9;
  int eatTime = 10;
  int shmid;



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
  int total_queue_sem = 0;
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
  sem_post(&shmdata -> total_queue_sem);
  sem_post(&shmdata -> lock_sem);

  /*Wait in queue_sem until a cashier is ready */
  printf("Waiting in queue for cashier. Position(%d) \n", total_queue_sem);
  sem_wait(&shmdata -> queue_sem);

}
