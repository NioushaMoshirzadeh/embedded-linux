#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "HandleTCPClient.h"

void HandleTCPClient (int clntSocket)
{
    int running = 1;
    char message[256];

    /* Clear window and show chatroom welcome message */
    printf("\e[1;1H\e[2J"); // clear console window
    printf("%s\n", chatroom_welcome); // print chatroom msg at top

    /*
     * Main program loop
     */
    while (running == 1)
    {
       /* Get message from client */
       if (recv(clntSocket, message, 32, 0) == -1)
       {
           printf("ERROR RECEIVING MESSAGE\n");
       }
       else
       {
           printf("Client: %s\n", message);
       }

       /* Get user input */
       printf("Enter Message: "); // enter server msg
       fgets(message, 256, stdin);
        
        /* Remove trailing newline character */
        if ((strlen(message) > 0) && (message[strlen(message) -1] == '\n'))
        {
            message[strlen(message) -1 ] = '\0';
        }

        /* if quit end program */
        if (strcmp(message, "quit") == 0)
        {
            printf("Quitting.. \n");
            running = 0;
            break;
        }
        else
        {
            /* Send input to client */
            printf("\033[1A"); // move up 1 line in console
            printf("%c[2K", 27); // clear current line in console
            if (send(clntSocket, message, strlen(message) + 1, 0) == -1)
            {
                printf("ERROR SENDING MESSAGE\n");
            }
            else
            {
                printf("Server: %s\n", message);
            }
        }
    }
    close (clntSocket);    /* Close client socket */
    info ("close");
}
