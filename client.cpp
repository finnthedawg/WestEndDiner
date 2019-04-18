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



  sem_t *sem = sem_open(SNAME1, 0);
  int value = 0;
  if (sem_getvalue(sem, &value) == -1){
    perror("Could not get value of semaphore");
  }
  cout << "Semaphore value is: " << value << endl;
}
