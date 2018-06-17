#include <stdio.h>
#include <stdlib.h>     // for atoi() and exit()
#include <unistd.h>     // for fork()
#include <sys/wait.h>   // for waitpid()
#include <errno.h>

#include "libs/Auxiliary.h"
#include "libs/AcceptTCPConnection.h"
#include "libs/CreateTCPServerSocket.h"
#include "libs/HandleTCPClient.h"

int main(int argc, char *argv[])
{
    int     servSock;                  /* Socket descriptor for server */
    int     clntSock;                  /* Socket descriptor for client */
    pid_t   processID;                 /* Process ID from fork() */
    bool    to_quit = false;

    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);

    /*
     * Main program loop
     * wait for client to connect
     * once connected, create new for to handle client
    */
    while (to_quit == false)
    {
        /* Wait for client to connect */
        clntSock = AcceptTCPConnection (servSock);

        /* Create fork */
        processID = fork();
        if (processID < 0)
        {
            perror("fork() failed");
            exit(1);
        }
        else
        {
            /* Main program */
            if (processID != 0)
            {
                printf("%x Main program\n", getpid());
            }
            /* Client program */
            else
            {
                printf("%x Created child\n", getpid());
                HandleTCPClient(clntSock);
            }
        }

    }

    // server stops...
	return (0);
}
