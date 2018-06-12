####################################################################
PROGRAM=main
CC = gcc
ACC = arm-linux-gcc
FLAGS = -Wall -Werror -lpthread
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

####################################################################
.PHONY: all upload clean
