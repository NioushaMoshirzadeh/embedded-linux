CC = arm-linux-gcc
FLAGS = -O2 -Wall -Werrork
LDFLAGS = -lusb-1.0

.PHONY: all upload

all: helloArgv.o helloArgv 

helloArgv.o: helloArgv.c
	$(CC) -c -o helloArgv.o helloArgv.c $(LDFLAGS)
	@echo Compiled: HelloArgv.c \>\> helloArgv.o

helloArgv: helloArgv.o
	$(CC) -o helloArgv helloArgv.o $(LDFLAGS)
	@echo Linked: helloArgv.o  \>\> helloArgv

upload:
	sshpass -p 'root' scp helloArgv root@10.0.0.4:~
	@echo Uploaded: helloArgv \>\> 10.0.0.4:~
