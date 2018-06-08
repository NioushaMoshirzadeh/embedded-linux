####################################################################
PROGRAM_ONE=p0
PROGRAM_TWO=p1
EXTRA=shm
CC = gcc
FLAGS = -Wall -Werror -lrt
DIR_BUILD = builds/
####################################################################

all: $(DIR_BUILD)$(PROGRAM_TWO) $(DIR_BUILD)$(PROGRAM_ONE) 

####################################################################

# compile
$(DIR_BUILD)$(EXTRA).o: $(EXTRA).c
	$(CC) -c -o $@ $^ $(FLAGS)

$(DIR_BUILD)$(PROGRAM_TWO).o: $(PROGRAM_TWO).c
	$(CC) -c -o $@ $^ $(FLAGS)

$(DIR_BUILD)$(PROGRAM_ONE).o: $(PROGRAM_ONE).c
	$(CC) -c -o $@ $^ $(FLAGS)


#link
$(DIR_BUILD)$(PROGRAM_TWO): $(DIR_BUILD)$(PROGRAM_TWO).o $(DIR_BUILD)$(EXTRA).o
	$(CC) $^ -o $@ $(FLAGS)
$(DIR_BUILD)$(PROGRAM_ONE): $(DIR_BUILD)$(PROGRAM_ONE).o $(DIR_BUILD)$(EXTRA).o
	$(CC) $^ -o $@ $(FLAGS)

clean:
	rm $(DIR_BUILD)*

####################################################################

.PHONY: all clean
