#include <iostream>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#include "shared.h"

using namespace std;

void printOrders(struct clientData clients[TOTALPEOPLE]);

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

  /* Initialize semaphores in shared memory*/
  if (sem_init(&shmdata -> coordinator_sem,1,0) == -1){
    perror("Failed to initialize coordinator_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> total_cashier_queue_sem,1,0) == -1){
    perror("Failed to initialize total_cashier_queue_sem");
    exit(-1);
  }
  if (sem_init(&shmdata -> totalserved_sem,1,0) == -1){
    perror("Failed to initialize totalserved_sem");
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
  for (int i = 0; i < TOTALPEOPLE; i ++){
    shmdata->clients[i].pid = -1;
  }
  D printf("Initialized all clients. \n");
  printf("Completed initialization. shmid is: %d\n",shmid);

  cin >> waitInput;
  /* After the last client has left, create summary information */
  printOrders(shmdata->clients);


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
    printf("Time in shop: %d", clients[0].time_in_shop);
    printf("Waited for cashier %d(s) cook %d(s) and server %d(s)\n", clients[i].time_cashier_waiting, clients[i].time_food_waiting, clients[i].time_server_waiting);
  }
}
