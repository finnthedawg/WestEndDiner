#include<iostream>
#include<string>

using namespace std;

int main() {
  string test;
  cin >> test;
  cout << test;
  size_t shmSize = 4096;
  int shmId = shmget(ipckey,shmSize,IPC_CREAT|perm);
}
