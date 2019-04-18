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

/* Menu: itemID,Description,Price,Min_time,Maxtime */
/* Searches for itemID from our  menuList and returns the description*/
char* findDescriptionMenu(int Id, std::vector<struct item> &menuList){
  for (int i = 0; i < menuList.size(); i++){
    if (menuList[i].itemId == Id){
      return(menuList[i].description);
    }
  }
  return(nullptr);
}



/*Adds a single item to our item vector*/
void loadMenu(const char* filename, std::vector<struct item> &menuList){
  FILE *fp = fopen(filename,"r");
  char* itemLine = (char*)malloc(STRLEN);
  if(fp != NULL){
    while(fgets(itemLine, STRLEN, fp)){
      menuList.push_back(createStruct(itemLine));
    }
  }
  free(itemLine);
  fclose(fp);

}

/*Creates a struct given a item line*/
/*Format of itemline: itemID,Description,Price,Min_time,Maxtime */
static struct item createStruct(char* itemLine){
  struct item x;
  const char delim = ',';
  /*Parse the values into struct x */
  x.itemId = atoi(strsep(&itemLine,&delim));
  strcpy(x.description, strsep(&itemLine,&delim));
  x.price = std::stof(strsep(&itemLine,&delim));
  x.min_time = atoi(strsep(&itemLine,&delim));
  x.max_time = atoi(strsep(&itemLine,&delim));
  return(x);
}
