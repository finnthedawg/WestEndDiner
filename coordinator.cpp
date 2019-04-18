#include<iostream>
#include<string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "shared.h"

using namespace std;

int main(int argc, char *argv[]) {

  /* Initialize our shared memory segment */
  int shmid; /* return value from shmget() */
  void *shmem; /* shmat returns pointer shmaddr to head of shared segment*/

  if ((shmid = shmget (SHMKEY, SHMSIZE, PERMS | IPC_CREAT)) < 0){
    perror("Shmget could not create segment");
    exit(1);
  }
  printf("Creating shared memory with SHMID: %d\n",shmid);

  if ((shmem = shmat(shmid, NULL, 0)) == (char *)-1){
    perror("shmat could not attach");
    exit(1);
  }
  printf("Attached shared memory with SHMID: %d\n",shmid);

  /* Create the required semaphores */



}
