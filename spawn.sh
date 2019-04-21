#!/bin/bash
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./cashier -s 5 -b 10
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./cashier -s 3 -b 3
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./server -s 3 -b 3
#Now spawn the clients
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 5 -e 3
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 8 -e 4
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 19 -e 6
sleep 10
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 7 -e 3
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 3 -e 3
sleep 10
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 3 -e 3
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 7 -e 3
sleep 10
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 19 -e 2
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 8 -e 2
sleep 10
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 8 -e 3
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 7 -e 2
sleep 10
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 7 -e 2
x-terminal-emulator -e /home/finn/Desktop/OS/diner/WestEndDiner/keepalive.sh ./client -i 7 -e 3
#Spawning and keeping terminals open https://askubuntu.com/questions/46627/how-can-i-make-a-script-that-opens-terminal-windows-and-executes-commands-in-the
