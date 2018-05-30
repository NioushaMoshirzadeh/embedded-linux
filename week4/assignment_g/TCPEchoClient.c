#include <stdio.h>
#include <string.h>     // for strlen()
#include <stdlib.h>     // for exit()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "libs/Auxiliary.h"
#include "libs/CreateTCPClientSocket.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

int main (int argc, char *argv[])
{
    int         sock;                   /* Socket descriptor */
    char *      echoString;             /* String to send to echo server */
    char        echoBuffer[RCVBUFSIZE]; /* Buffer for received string */
    int         echoStringLen;          /* Length of string to echo */
    int         bytesRcvd;              /* Bytes read in single recv() */
    int         i;                      /* counter for data-arguments */

    parse_args (argc, argv);

    sock = CreateTCPClientSocket (argv_ip, argv_port);
        
    for (i = 0; i < argv_nrofdata; i++)
    {
        echoString = argv_data [i];
        echoStringLen = strlen (echoString) + 1;          
        /* Determine input length */

        delaying();
        
        /* Send string to the server */
        send(sock, echoString, echoStringLen, 0);

        /* Display transmitted string in verbose mode */
        info_s("Sent string", echoString);
        
        // TODO: add code to receive & display the converted string from the server
        bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE, 0);
        printf("Received bytes: %d\nReceived message: %s\n", bytesRcvd, echoBuffer);
        //for (int i=0; i<bytesRcvd; i++)
        //{
            //printf("%c", echoBuffer[i]);
        //}
        //printf("\n");
        
    }

    delaying ();

    close (sock);
    info ("close & exit");
    exit (0);
}


