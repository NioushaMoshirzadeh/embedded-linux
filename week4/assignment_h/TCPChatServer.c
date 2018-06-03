#include "libs/Auxiliary.h"
#include "libs/AcceptTCPConnection.h"
#include "libs/CreateTCPServerSocket.h"
#include "libs/HandleTCPClient.h"

int main (int argc, char * argv[])
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */

    parse_args (argc, argv);
    
    servSock = CreateTCPServerSocket (argv_port);

    for (;;) /* Run forever */
    {
        clntSock = AcceptTCPConnection (servSock);
        HandleTCPClient (clntSock);
    }
    /* NOT REACHED */
}
