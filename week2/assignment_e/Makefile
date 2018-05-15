TARGET=hello
CC = gcc
ACC = arm-linux-gcc
FLAGS = -Wall -Werror -lrt
DIR_LOCAL=builds/local/
DIR_TARGET=builds/target/

.PHONY: all clean

all: $(TARGET)
	
# Compile target
$(TARGET).o: $(TARGET).c
	$(CC) -c $^ -o $(DIR_LOCAL)$@ $(FLAGS)
	$(ACC) -c $^ -o $(DIR_TARGET)$@ $(FLAGS)

# Link target
$(TARGET): $(TARGET).o
	$(CC) $(DIR_LOCAL)$(TARGET).o -o $(DIR_LOCAL)$@ $(FLAGS)
	$(ACC) $(DIR_LOCAL)$(TARGET).o -o $(DIR_LOCAL)$@ $(FLAGS)

clean:
	rm -rf $(DIR_LOCAL)* $(DIR_TARGET)*
