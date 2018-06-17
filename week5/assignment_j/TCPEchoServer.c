#include <stdio.h>
#include "libs/Auxiliary.h"
#include "libs/AcceptTCPConnection.h"
#include "libs/CreateTCPServerSocket.h"
#include "libs/HandleTCPClient.h"
#include <pthread.h>


/* Socket descriptor for server */
volatile int servSock;
/* Socket descriptor for client */
volatile int clntSock;
volatile int clientNumber = 1;

/*
 * Client thread
 */
static void* clientThread(void* arg)
{
    int threadNr;

    threadNr = (int) (long) arg;
    printf("%lx clientThread start #%d\n", pthread_self(), threadNr);

    /*
     * Mark thread as detached, so that resources are freed once it
     * finishes execution
     */
    if (pthread_detach(pthread_self()) != 0)
    {
        perror("pthread_detach(a)");
    }

    HandleTCPClient (clntSock, threadNr);

    printf("%lx clientThread exit\n", pthread_self());
    return (NULL);
}

int main (int argc, char * argv[])
{
    pthread_t thread_client;

    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);

    /*
     * Main program loop
     * waits for client to connect
     * then passes handling of client off to a thread
     * and main program returns to waiting for a client to connect
     */
    for (;;) /* Run forever */
    {
        /* Wait for client to connect */
        clntSock = AcceptTCPConnection (servSock);

        /* Create thread to handle client */
        if (pthread_create(&thread_client, NULL, clientThread, (void *) (long) clientNumber) != 0)
        {
            perror("Error creating thread");
        }
        else
        {
            clientNumber++;
        }
    }
    /* NOT REACHED */
}
