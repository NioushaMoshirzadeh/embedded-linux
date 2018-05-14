CC = gcc
FLAGS = -Wall -Werror -lrt
DIR_LOCAL=builds/local/
DIR_TARGET=builds/target/

.PHONY: all A B clean

all: A B

delay.o: delay.c
	$(CC) -c delay.c -o $(DIR_LOCAL)delay.o $(FLAGS)
	
# Compile and link program A
A: progA

# Compile program A
progA.o: progA.c
	$(CC) -c $^ -o $(DIR_LOCAL)$@ $(FLAGS)

# Link program A
progA: progA.o delay.o
	$(CC) $(DIR_LOCAL)progA.o $(DIR_LOCAL)delay.o -o $(DIR_LOCAL)$@ $(FLAGS)

# Compile and link program B
B: progB

# Compile program B
progB.o: progB.c
	$(CC) -c $^ -o $(DIR_LOCAL)$@ $(FLAGS)

# Link program B
progB: progB.o delay.o
	$(CC) $(DIR_LOCAL)progB.o $(DIR_LOCAL)delay.o -o $(DIR_LOCAL)$@ $(FLAGS)

clean:
	rm -rf $(DIR_LOCAL)*.o progA progB
