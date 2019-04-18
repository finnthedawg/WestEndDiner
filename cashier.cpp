#include <iostream>
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "shared.h"

using namespace std;
/* Options: ./client -s serviceTime -b breakTime -m shmid */
int main(int argc, char *argv[]) {

  /* Some default cashier values */
  int serviceTime = 10;
  int breakTime = 10;
  int shmid = -1; //-1 means using SHMKEY to generate the SHM rather than shmid

  /* Parse our arguments */
  int opt;
  while((opt = getopt(argc, argv, "s:b:m:")) != -1){
    switch(opt){
      case 's':
        printf("serviceTime set to %s\n", optarg);
        serviceTime = stoi(optarg);
        break;
      case 'b':
        printf("Max breakTime set to %s\n", optarg);
        breakTime = stoi(optarg);
        break;
      case 'm':
        printf("shmid set to %s\n", optarg);
        shmid = stoi(optarg);
        break;
      case '?':
        printf("Unknown flag: %s\n", optarg);
        break;
    }
  }
  srand(time(0)); //randomize with time seed
  serviceTime = rand() % serviceTime;
  breakTime = rand() % breakTime;

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
    sem_wait(&shmdata -> cashier_lock_sem); //Acquire lock for cashier.
    int total_cashier_queue_sem;
    if (sem_getvalue(&shmdata -> total_cashier_queue_sem, &total_cashier_queue_sem) == -1){
      perror("Could not get value of semaphore");
      sem_post(&shmdata -> cashier_lock_sem);
      exit(1);
    }
    /* If there are clients waiting then signal that cashier is ready. */
    if (total_cashier_queue_sem > 0){
      D printf("Cashier told customer to tell her order information.\n");
      sem_wait(&shmdata -> total_cashier_queue_sem); //Removes this client from the queue.
      sem_post(&shmdata -> cashier_queue_sem); //Tells client to come over.
      sem_wait(&shmdata -> cashier_signal); //Wait for the client to come over and submit order (PID info)

      D printf("Customer submitted information\n");
      int pid = shmdata -> clientpid; //Remember this client.
      sem_post(&shmdata -> cashier_lock_sem); //Unlock cashiers and lets other cashiers call people.

      printf("Servicing the client: %d.\n", pid);
      sleep(serviceTime); //Serve the client.
      sem_post(&getClientById(pid, shmdata->clients)->paid_sem);
      printf("Done servicing the client: %d\n",pid);
    } else {
      /* Otherwise, we take a break and unlock control.*/
      D printf("There is nobody in queue, taking a break. \n");
      sem_post(&shmdata -> cashier_lock_sem); //Unlock this cashier
      sleep(breakTime);
    }

  }

}
