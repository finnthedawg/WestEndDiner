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

  sem_t *sem=sem_open(SNAME1, O_CREAT, 0644, 0);
  if (sem == SEM_FAILED){
    perror("Failed to open semaphore");
    exit(-1);
  }
  sem_post(sem);
  printf("Created and incremented semaphore: %s\n",SNAME1);
  int value;
  if (sem_getvalue(sem, &value) == -1){
    perror("Could not get value of semaphore");
  }
  cout << "Semaphore value is: " << value << endl;



}
