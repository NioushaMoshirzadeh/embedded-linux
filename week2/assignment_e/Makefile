####################################################################
TARGET=hello
CC = gcc
ACC = arm-linux-gcc
FLAGS = -Wall -Werror -lrt
DIR_LOCAL=builds/local/
DIR_TARGET=builds/target/
####################################################################

all: $(DIR_LOCAL)$(TARGET) $(DIR_TARGET)$(TARGET)
	
# LOCAL
$(DIR_LOCAL)$(TARGET).o: $(TARGET).c
	$(CC) -c $^ -o $@ $(FLAGS)
$(DIR_LOCAL)$(TARGET): $(DIR_LOCAL)$(TARGET).o
	$(CC) $^ -o $@ $(FLAGS)

# TARGET
$(DIR_TARGET)$(TARGET).o: $(TARGET).c
	$(ACC) -c $^ -o $@ $(FLAGS)
$(DIR_TARGET)$(TARGET): $(DIR_TARGET)$(TARGET).o
	$(ACC) $^ -o $@ $(FLAGS)

# upload to device as root@10.0.0.4
upload:
	sshpass -p 'root' scp $(DIR_TARGET)$(TARGET) root@10.0.0.4:~

clean:
	rm -rf $(DIR_LOCAL)* $(DIR_TARGET)*

####################################################################
.PHONY: all upload clean
