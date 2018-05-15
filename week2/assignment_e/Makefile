####################################################################
TARGET=
CC = gcc
ACC = arm-linux-gcc
FLAGS = -Wall -Werror -lrt
DIR_LOCAL=builds/local/
DIR_TARGET=builds/target/

####################################################################

all: $(TARGET)
	
# Compile
$(TARGET).o: $(TARGET).c
	$(CC) -c $^ -o $(DIR_LOCAL)$@ $(FLAGS)
	$(ACC) -c $^ -o $(DIR_TARGET)$@ $(FLAGS)

# Link
$(TARGET): $(TARGET).o
	$(CC) $(DIR_LOCAL)$(TARGET).o -o $(DIR_LOCAL)$@ $(FLAGS)
	$(ACC) $(DIR_TARGET)$(TARGET).o -o $(DIR_TARGET)$@ $(FLAGS)

# upload to device as root @10.0.0.4
upload:
	sshpass -p 'root' scp helloArgv root@10.0.0.4:~

clean:
	rm -rf $(DIR_LOCAL)* $(DIR_TARGET)*

####################################################################

.PHONY: all upload clean
