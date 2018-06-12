#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()
#include <semaphore.h>

//#include <libusb-1.0/libusb.h>
#include "xboxHandler.h"
#include "Auxiliary.h"
#include "HandleTCPClient.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

unsigned char BTN_INPUTS[34] = {0, 34, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char RUMBLE_ON[8]   = {0, 8, 0, 0x88, 0x88, 0, 0, 0};
unsigned char RUMBLE_OFF[8]  = {0, 8, 0, 0x00, 0x00, 0, 0, 0}; 
unsigned char LED_ALL_ON[3]  = {0x01, 0x03, 0x01};
unsigned char LED_ALL_OFF[3] = {0x01, 0x03, 0x00};

void HandleTCPClient (int clntSocket, sem_t* sem)
{
    // 'clntSocket' is obtained from AcceptTCPConnection()

    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int  recvMsgSize;                   /* Size of received message */
    libusb_device_handle* h = NULL;

    if (h == NULL)
    {
        printf("Error connecting to device\n");
    }
    else
    {
        printf("Connected to device\n");
    }
    /* Receive message from client */
    recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
    if (recvMsgSize < 0)
    {
        DieWithError ("recv() failed");
    }
    info_d ("Recv", recvMsgSize);

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        info("Waiting..");
        sem_wait(sem);
        h = openDevice();
        /* Print the received message */
        printf("Received message: %s\n", echoBuffer);
        
        switch (echoBuffer[0])
        {
            case '1':
                printf("Got 1\n"); 
                sendCommand(h, RUMBLE_ON, 8);
                break;
            case '2':
                printf("Got 2\n"); 
                sendCommand(h, RUMBLE_OFF, 8);;
                break;
            case '3':
                printf("Got 3\n"); 
                sendCommand(h, LED_ALL_ON, 8);
                break;
            case '4':
                printf("Got 4\n"); 
                sendCommand(h, LED_ALL_OFF, 8);
                break;
            default:
                break;
        }

        sleep(5);
        info("closing..");
        libusb_close(h);
        delaying ();
        
        /* Echo message back to client */
        if (send (clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
        {
            DieWithError ("send() failed");
        }

        info("posting\n");
        sem_post(sem);

        /* Display transmitted string in verbose mode */
        info_s("Sent string", echoBuffer);

        // receive next string
        recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
        if (recvMsgSize < 0)
        {
            DieWithError ("recv() failed");
        }
        info_d ("recv", recvMsgSize);
    }

    close (clntSocket);    /* Close client socket */
    info ("close");
}
