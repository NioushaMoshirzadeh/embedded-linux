#include <stdio.h>
#include <string.h>     // for strlen()
#include <stdlib.h>     // for exit()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "libs/Auxiliary.h"
#include "libs/CreateTCPClientSocket.h"

int main (int argc, char *argv[])
{
    int         sock;                   /* Socket descriptor */
    int         running = 1; 
    char        message[MAX_MSG_SIZE];

    /* parse command line arguments */
    parse_args (argc, argv);

    /* Create socket */
    sock = CreateTCPClientSocket (argv_ip, argv_port);

    /* Clear window and show chatroom welcome message */
    printf("\e[1;1H\e[2J"); // clear console window
    printf("%s\n", chatroom_welcome);

    /*
     * Main program loop
     */
    while (running == 1)
    {
        /* Get user message input */
        printf("Enter message: ");
        fgets(message, MAX_MSG_SIZE, stdin);

        /* Remove trailing newline character from user input */
        if ((strlen(message) > 0) && (message[strlen(message) -1] == '\n'))
        {
            message[strlen(message) -1 ] = '\0';
        }

        /* If message is quit, end program */
        if (strcmp(message, "quit") == 0)
        {
            printf("Quitting.. \n");
            running = 0;
            break;
        }
        else
        {
            /* Send the message and show the message the user sent */
            printf("\033[1A"); // move up 1 line in console
            printf("%c[2K", 27); // clear current lin in console
            if (send(sock, message, strlen(message) + 1, 0) == -1)
            {
                printf("ERROR SENDING MESSAGE\n");
            }
            else
            {
                printf("You: %s\n", message);
            }

            /* Wait for message from the server */
            if (recv(sock, message, 32, 0) == -1)
            {
                printf("ERROR RECEIVING MESSAGE\n");
            }
            else
            {
                printf("Other: %s\n", message);
            }
        }
    }

    /* diconnect from socket and end program */
    delaying();
    close(sock);
    info("close & exit");
    exit(0);
}


