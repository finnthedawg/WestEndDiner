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

  /* Create SHM with our shared SHMKEY*/
  if ((shmid = shmget (SHMKEY, sizeof(struct sharedData), PERMS | IPC_CREAT)) < 0){
    perror("Shmget could not create segment");
    exit(1);
  }
  printf("Creating shared memory with SHMID: %d\n",shmid);

  /* Attaches the shared memory with id shmid*/
  if ((shmaddr = shmat(shmid, NULL, 0)) == (char *)-1){
    perror("shmat could not attach");
    exit(1);
  }
  printf("Attached shared memory with SHMID: %d\n",shmid);

  /* Convert shmaddr pointer to our data pointer. */
  shmdata = (struct sharedData *)shmaddr;
  shmdata -> name = "Testing";

  /*Sends command to the SHM. IPC RMID = mark the segment to be destroyed*/
  if (shmctl(shmid, IPC_RMID, 0) == -1){
    perror("shm could be removed");
    exit(1);
  }
  printf("Marked memory with SHMID to be removed when last process detaches: %d\n",shmid);

  /* Detach our shared memory */
  if (shmdt(shmaddr) == -1){
    perror("shm could not detach");
    exit(1);
  }
  printf("Detached shared memory with SHMID: %d\n",shmid);

  /* Initialize our semaphores */
  sem_t *sem=sem_open(SNAME1, O_CREAT, 0644, 10);
  if (sem == SEM_FAILED){
    perror("Failed to open semaphore");
    exit(-1);
  }
  printf("Created and incremented semaphore: %s\n",SNAME1);
  int value;
  if (sem_getvalue(sem, &value) == -1){
    perror("Could not get value of semaphore");
  }
  cout << "Semaphore value is: " << value << endl;

}
