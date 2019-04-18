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
  D cout << shmdata -> name << endl;

  int value = 0;
  if (sem_getvalue(&shmdata -> coordinator_sem, &value) == -1){
    perror("Could not get value of semaphore");
  }
  cout << "Semaphore value is: " << value << endl;
}
