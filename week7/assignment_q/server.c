#include <stdio.h>
#include <stdlib.h>     // for atoi() and exit()
#include <unistd.h>     // for fork()
#include <sys/wait.h>   // for waitpid()
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>

#include "libs/Auxiliary.h"
#include "libs/AcceptTCPConnection.h"
#include "libs/CreateTCPServerSocket.h"
#include "libs/HandleTCPClient.h"


#define SEM_NAME        "XBOX_SEMAPHORE"
#define SEM_NAME_FULL   "/dev/shm/XBOX_SEMAPHORE"

const char* sem_name = SEM_NAME;

int main(int argc, char *argv[])
{
    int     servSock;                  /* Socket descriptor for server */
    int     clntSock;                  /* Socket descriptor for client */
    pid_t   processID;                 /* Process ID from fork() */
    bool    to_quit = false;
    sem_t*  sem = SEM_FAILED;

    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);

    while (to_quit == false)                /* run until someone indicates to quit... */
    {

        clntSock = AcceptTCPConnection (servSock);

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
                info_d("Main pid", getpid());
                info_d("Main process", processID);
                remove(SEM_NAME_FULL);
                sem = sem_open(sem_name, O_CREAT|O_EXCL,0600,1);
            }
            else
            {
                info_d("child process", getpid());
                sem = sem_open(sem_name, 0);
                HandleTCPClient(clntSock, sem);
            }
        }
    }

    // server stops...
	return (0);
}
