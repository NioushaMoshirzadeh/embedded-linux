####################################################################
PROGRAM=main
CC = gcc
DIR=builds/
FLAGS = -Wall -Werror -lpthread
####################################################################

$(DIR)$(PROGRAM): $(DIR)$(PROGRAM).o
	$(CC) $^ -o $@ $(FLAGS)

$(DIR)$(PROGRAM).o: $(PROGRAM).c
	$(CC) -c $^ -o $@ $(FLAGS)


clean:
	rm -rf $(DIR)*

####################################################################
.PHONY: clean
