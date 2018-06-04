#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "HandleTCPClient.h"

#define RCVBUFSIZE 250   /* Size of receive buffer */
char* chatroom_welcome =
"#########################################################\n"
"#                                                       #\n"
"#                       Chat room                       #\n"
"#                                                       #\n"
"#########################################################\n";

void HandleTCPClient (int clntSocket)
{
    // 'clntSocket' is obtained from AcceptTCPConnection()

    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int  recvMsgSize;                   /* Size of received message */
    int running = 1;
    char message[256];

    printf("\e[1;1H\e[2J"); // clear console window
    printf("%s\n", chatroom_welcome); // print chatroom msg at top
    while (running == 1)
    {
       recv(clntSocket, message, 32, 0);  // get msg from client
       printf("Client: %s\n", message); // show msg
       printf("Enter Message: "); // enter server msg
       fgets(message, 256, stdin);
        
        /* Remove trailing newline character */
        if ((strlen(message) > 0) && (message[strlen(message) -1] == '\n'))
        {
            message[strlen(message) -1 ] = '\0';
        }

        if (strcmp(message, "quit") == 0)
        {
            printf("Quitting.. \n");
            running = 0;
            break;
        }
        else
        {
            printf("\033[1A"); // move up 1 line in console
            printf("%c[2K", 27); // clear current line in console
            printf("Server: %s\n", message);
            send(clntSocket, message, strlen(message) + 1, 0);
        }
    }
    close (clntSocket);    /* Close client socket */
    info ("close");

    /* Receive message from client */
    recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
    if (recvMsgSize < 0)
    {
        DieWithError ("recv() failed");
    }
    info_d ("Recv", recvMsgSize);


}
