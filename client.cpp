#include <iostream>
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#include "shared.h"

using namespace std;

int main(int argc, char *argv[]) {

  /* Initialize our shared memory segment */
  struct sharedData *shmdata; //Our data struct stored in shared memory
  int shmid;
  void *shmaddr; /* Pointer to head of shm*/

  /* Load our previous SHM associated with SHMKEY*/
  if ((shmid = shmget (SHMKEY, sizeof(struct sharedData), 0)) < 0){
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
