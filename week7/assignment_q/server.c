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

    while (to_quit == false)                /* run until someone indicates to quit... */
    {
        //info_d("main program start", getpid());

        clntSock = AcceptTCPConnection (servSock);

        processID = fork();
        //info_d("Forked", processID);

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
                info_d("Main pid", getpid());
                info_d("Main process", processID);
            }
            else
            {
                info_d("child process", getpid());
                HandleTCPClient(clntSock);
            }
        }

    }
    
    // server stops...
	return (0);
}
