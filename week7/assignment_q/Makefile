####################################################################
CLIENT=client
SERVER=server
CC = gcc
ACC = arm-linux-gcc
FLAGS = -Wall -Werror -lrt -lusb-1.0 -lpthread
DIR_LOCAL=builds/local/
DIR_TARGET=builds/target/
DIR_LIB=libs/
####################################################################

all: $(DIR_LOCAL)$(CLIENT) $(DIR_LOCAL)$(SERVER) $(DIR_TARGET)$(SERVER)
	
####################################################################
#   LIBRARIES                                                      #
####################################################################
# local
$(DIR_LOCAL)xboxHandler.o: $(DIR_LIB)xboxHandler.c
	$(CC) -c $^ -o $@ $(FLAGS)
$(DIR_LOCAL)Auxiliary.o: $(DIR_LIB)Auxiliary.c
	$(CC) -c $^ -o $@ $(FLAGS)
$(DIR_LOCAL)CreateTCPClientSocket.o: $(DIR_LIB)CreateTCPClientSocket.c
	$(CC) -c $^ -o $@ $(FLAGS)
$(DIR_LOCAL)CreateTCPServerSocket.o: $(DIR_LIB)CreateTCPServerSocket.c
	$(CC) -c $^ -o $@ $(FLAGS)
$(DIR_LOCAL)AcceptTCPConnection.o: $(DIR_LIB)AcceptTCPConnection.c
	$(CC) -c $^ -o $@ $(FLAGS)
$(DIR_LOCAL)HandleTCPClient.o: $(DIR_LIB)HandleTCPClient.c
	$(CC) -c $^ -o $@ $(FLAGS)

LOCAL_LIBS=$(DIR_LOCAL)xboxHandler.o\
           $(DIR_LOCAL)Auxiliary.o\
           $(DIR_LOCAL)CreateTCPClientSocket.o\
           $(DIR_LOCAL)CreateTCPServerSocket.o\
           $(DIR_LOCAL)AcceptTCPConnection.o\
           $(DIR_LOCAL)HandleTCPClient.o\

# target
$(DIR_TARGET)xboxHandler.o: $(DIR_LIB)xboxHandler.c
	$(ACC) -c $^ -o $@ $(FLAGS)
$(DIR_TARGET)Auxiliary.o: $(DIR_LIB)Auxiliary.c
	$(ACC) -c $^ -o $@ $(FLAGS)
$(DIR_TARGET)CreateTCPClientSocket.o: $(DIR_LIB)CreateTCPClientSocket.c
	$(ACC) -c $^ -o $@ $(FLAGS)
$(DIR_TARGET)CreateTCPServerSocket.o: $(DIR_LIB)CreateTCPServerSocket.c
	$(ACC) -c $^ -o $@ $(FLAGS)
$(DIR_TARGET)AcceptTCPConnection.o: $(DIR_LIB)AcceptTCPConnection.c
	$(ACC) -c $^ -o $@ $(FLAGS)
$(DIR_TARGET)HandleTCPClient.o: $(DIR_LIB)HandleTCPClient.c
	$(ACC) -c $^ -o $@ $(FLAGS)

TARGET_LIBS=$(DIR_TARGET)xboxHandler.o\
            $(DIR_TARGET)Auxiliary.o\
            $(DIR_TARGET)CreateTCPClientSocket.o\
            $(DIR_TARGET)CreateTCPServerSocket.o\
            $(DIR_TARGET)AcceptTCPConnection.o\
            $(DIR_TARGET)HandleTCPClient.o\

####################################################################
#   MAIN PROGRAM                                                   #
####################################################################
# local
$(DIR_LOCAL)$(CLIENT).o: $(CLIENT).c
	$(CC) -c $^ -o $@ $(FLAGS)
$(DIR_LOCAL)$(CLIENT): $(DIR_LOCAL)$(CLIENT).o $(LOCAL_LIBS)
	$(CC) $^ -o $@ $(FLAGS)
$(DIR_LOCAL)$(SERVER).o: $(SERVER).c
	$(CC) -c $^ -o $@ $(FLAGS)
$(DIR_LOCAL)$(SERVER): $(DIR_LOCAL)$(SERVER).o $(LOCAL_LIBS)
	$(CC) $^ -o $@ $(FLAGS)

# target
$(DIR_TARGET)$(SERVER).o: $(SERVER).c
	$(ACC) -c $^ -o $@ $(FLAGS)
$(DIR_TARGET)$(SERVER): $(DIR_TARGET)$(SERVER).o $(TARGET_LIBS)
	$(ACC) $^ -o $@ $(FLAGS)

# upload to device as root@10.0.0.4
upload:
	sshpass -p 'root' scp $(DIR_TARGET)$(SERVER) root@10.0.0.4:~

clean:
	rm -rf $(DIR_LOCAL)* $(DIR_TARGET)*

####################################################################
.PHONY: all upload clean
