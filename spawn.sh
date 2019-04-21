#!/bin/bash
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./cashier -s 5 -b 10
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./cashier -s 3 -b 3
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./server
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client
#Spawning and keeping terminals open https://askubuntu.com/questions/46627/how-can-i-make-a-script-that-opens-terminal-windows-and-executes-commands-in-the
