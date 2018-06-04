#include <stdio.h>
#include <string.h>     // for strlen()
#include <stdlib.h>     // for exit()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "libs/Auxiliary.h"
#include "libs/CreateTCPClientSocket.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

// TODO: double declaration of chatroom_welcome??
char* chatroom_welcom =
"#########################################################\n"
"#                                                       #\n"
"#                       Chat room                       #\n"
"#                                                       #\n"
"#########################################################\n";

int main (int argc, char *argv[])
{
    int         sock;                   /* Socket descriptor */
    char *      echoString;             /* String to send to echo server */
    char        echoBuffer[RCVBUFSIZE]; /* Buffer for received string */
    int         echoStringLen;          /* Length of string to echo */
    int         bytesRcvd;              /* Bytes read in single recv() */
    int         i;                      /* counter for data-arguments */
    int         running = 1; 
    char message[250];

    parse_args (argc, argv);

    sock = CreateTCPClientSocket (argv_ip, argv_port);
    printf("\e[1;1H\e[2J"); // clear console window
    printf("%s\n", chatroom_welcom);
    while (running == 1)
    {
        printf("Enter message: ");
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
            printf("%c[2K", 27); // clear current lin in console
            printf("You: %s\n", message);
            send(sock, message, strlen(message) + 1, 0);
            recv(sock, message, 32, 0);
            printf("Other: %s\n", message);
        }
    }
    delaying();
    close(sock);
    info("close & exit");
    exit(0);
        
    for (i = 0; i < argv_nrofdata; i++)
    {
        echoString = argv_data [i];
        echoStringLen = strlen (echoString) + 1;    // Add 1 to string length to include \0
        /* Determine input length */

        delaying();
        
        /* Send string to the server */
        send(sock, echoString, echoStringLen, 0);

        /* Display transmitted string in verbose mode */
        info_s("Sent string", echoString);
        
        /* Display receive and converted string from server */
        bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE, 0);
        printf("Received bytes: %d\nReceived message: %s\n", bytesRcvd, echoBuffer);
    }
}


