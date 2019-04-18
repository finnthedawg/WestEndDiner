#include <iostream>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#include "shared.h"

using namespace std;

int main(int argc, char *argv[]) {
  string waitInput;
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
  sprintf(shmdata -> name, "Testing");

  /* Initialize semaphores in shared memory*/
  if (sem_init(&shmdata -> coordinator_sem,1,0) == -1){
    perror("Failed to initialize coordinator_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> total_queue_sem,1,0) == -1){
    perror("Failed to initialize total_queue_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> totalserved_sem,1,0) == -1){
    perror("Failed to initialize totalserved_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> queue_sem,1,0) == -1){
    perror("Failed to initialize queue_sem");
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
  if (sem_init(&shmdata -> server_queue_sem,1,1) == -1){
    perror("Failed to initialize server_queue_sem");
    exit(-1);
  }
  D printf("Initialized all semaphores: %d\n",shmid);

  /* Zero out the client array.*/
  shmdata -> numclients = 0;
  for (int i = 0; i < TOTALPEOPLE; i ++){
    shmdata->clients[i].pid = -1;
  }
  D printf("Initialized all clients. \n");
  printf("Completed initialization. shmid is: %d\n",shmid);

  cin >> waitInput;
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
