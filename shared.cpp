#include "shared.h"

/* Searches for ID in clientData array. Returns nullptr if not found */
struct clientData * getClientById(int Id, struct clientData* shmdata){
  for (int i = 0; i < TOTALPEOPLE; i++){
    if (shmdata->pid == Id){
      return(shmdata);
    }
    shmdata++;
  }
  return(nullptr);
}

/* Searches for itemID from our comma seperated menu. */
/* Format: itemID,Description,Price,Min_time,Maxtime */
char* findDescriptionMenu(int Id, char* path, struct item* items){
}
