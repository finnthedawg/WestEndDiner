#include "shared.h"
struct clientData * getClientById(int Id, struct clientData* shmdata){
  for (int i = 0; i < TOTALPEOPLE; i++){
    if (shmdata->pid == Id){
      return(shmdata);
    }
    shmdata++;
  }
  return(nullptr);
}
