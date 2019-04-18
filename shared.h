#define DEBUG
#ifdef DEBUG
 #define D if(1)
#else
 #define D if(0)
#endif

#define TOTALPEOPLE 5 //Total number of people that can be served by restaurant before shutting down
#define MAXQUEUE 3 //Maximum number of people in queue.
#define PERMS 0600 //Permission levels of shared memory
#define SHMKEY (key_t)6666 //Key used to create SHM
#define SEM_PERMS 0644 //Permission levels of semaphores.
#define STRLEN 1024
#include <semaphore.h>
/* Define some useful shared functions */
struct clientData * getClientById(int Id, struct clientData* shmdata);

/* Information about the client  */
struct clientData{
  int pid;
  sem_t paid_sem; //Cashier will signal when client has paid.
};

struct sharedData{
  char name[STRLEN];
  sem_t lock_sem; //Initialized to 1. Lock the sharedData ensuring only one process can use shared.
  sem_t cashier_lock_sem; //Initialized to 1. Lock the cashier so only one cashier is operating

  sem_t totalserved_sem; //Initialized to 0. Incremented by new clients when leaving. Last client checks if equal to TOTALPEOPLE
  sem_t coordinator_sem; //Initialized to 0. Signal by last client.

  sem_t total_queue_sem; //Initialized to 0. Incremented by clients entering queue. Decremented by cashier.
  sem_t cashier_queue_sem; //Initialized to 0. Decremented by clients in queue. Incremented by cashier when ready.
  sem_t cashier_signal; //Initialized to 0. Cashier waits for the chosen client to finish submitting clientpid.
  int clientpid; //clientpid will be put here awaiting for cashier to retrieve.

  sem_t server_queue_sem; //Initialized to 0. Decrements by clients in queue.
  sem_t server_signal; //Initialized to 0. Server waits for the chosen client to finish submitting serverClientPid.
  int serverClientPid; //serverClientPid will be put here awaiting for server to retrieve.

  int numclients; //Number of clients
  struct clientData clients[TOTALPEOPLE]; //Client info is found here
};
