#define DEBUG
#ifdef DEBUG
 #define D if(1)
#else
 #define D if(0)
#endif

#define TOTALPEOPLE 10 //Total number of people that can be served by restaurant before shutting down
#define PERMS 0600 //Permission levels of shared memory
#define SHMKEY (key_t)6666 //Key used to create SHM
#define SEM_PERMS 0644 //Permission levels of semaphores.
#define STRLEN 1024
#include <semaphore.h>


/* Information about the client  */
struct clientData{
  int clientPid;
  sem_t cashier_sem; // Initialized to 0 for each client. Incremented by cashier.
};

struct sharedData{
  char name[STRLEN];
  sem_t coordinator_sem; //Initialized to 0. Signal when last client leaves.
  sem_t total_queue_sem; //Initialized to 0. Incremented by clients entering queue. Decremented by cashier.
  sem_t queue_sem; //Intialized to 0. Decremented by new clients to wait. Incremented by cashier.
  sem_t totalserved_sem; //Initialized to 0. Incremented by new clients when leaving. Last client checks if equal to TOTALPEOPLE
  struct clientData cashierSHM; //ClientData will be put here awaiting for cashier to retrieve.
  struct clientData clients[TOTALPEOPLE]; //Summary information of clients.
};
