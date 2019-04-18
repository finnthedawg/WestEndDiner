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

int main(int argc, char *argv[]) {

  /* Some default server values */
  int serviceTime = 5;
  int breakTime = 5;
  int shmid = -1; //-1 means using SHMKEY to generate the SHM rather than shmid

  /* Initialize our shared memory segment */
  struct sharedData *shmdata; //Our data struct stored in shared memory

  /* Load our previous SHM associated with SHMKEY*/
  if ((shmid = shmget (SHMKEY, sizeof(struct sharedData), 0)) < 0){
    perror("Shmget could not obtain segment");
    exit(1);
  }
  D printf("Obtaining shared memory with SHMID: %d\n",shmid);

  /* Attaches the shared memory with id shmid*/
  if ((shmdata = (struct sharedData *)shmat(shmid, NULL, 0)) == (struct sharedData *)-1){
    perror("shmat could not attach");
    exit(1);
  }

   while(1){
     int total_server_queue_sem;
     if (sem_getvalue(&shmdata -> total_server_queue_sem, &total_server_queue_sem) == -1){
       perror("Could not get value of total_server_queue_sem");
       exit(1);
     }
     /* If there are clients waiting then signal that server is ready. */
     if (total_server_queue_sem > 0){
       D printf("Server told client to tell her order information.\n");
       sem_wait(&shmdata -> total_server_queue_sem); //Decrement total_server_queue_sem
       sem_post(&shmdata -> server_queue_sem); //Tells client to come over.
       /* serve the client */
       sleep(serviceTime);
       sem_post(&shmdata -> client_signal);
       printf("Done servicing the client.\n");
     } else {
       /* Otherwise, we take a break and unlock control.*/
       D printf("There is nobody in queue, taking a break. \n");
       sleep(breakTime);
     }
  }


}
