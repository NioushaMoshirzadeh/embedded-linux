####################################################################
PROGRAM=main
CC = gcc
ACC = arm-linux-gcc
FLAGS = -Wall -Werror -lrt -lpthread
DIR=builds/
####################################################################

all: $(DIR)$(PROGRAM)

#compile
$(DIR)$(PROGRAM).o: $(PROGRAM).c
	$(CC) -c $^ -o $@ $(FLAGS)

#link
$(DIR)$(PROGRAM): $(DIR)$(PROGRAM).o
	$(CC) $^ -o $@ $(FLAGS)

clean:
	rm -rf $(DIR)*
	rm /dev/shm/sem.sem1
	rm /dev/shm/sem.sem2

####################################################################
.PHONY: all upload clean
