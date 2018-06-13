#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()
#include <semaphore.h>

#include "xboxHandler.h"
#include "Auxiliary.h"
#include "HandleTCPClient.h"

#define RCVBUFSIZE 256   /* Size of receive buffer */


void GenerateOutputMessage();

void HandleTCPClient (int clntSocket, sem_t* sem)
{
    // 'clntSocket' is obtained from AcceptTCPConnection()

    char echoBuffer[RCVBUFSIZE ];        /* Buffer for echo string */
    int  recvMsgSize;                   /* Size of received message */
    libusb_device_handle* h = NULL;

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
        if (h == NULL)
        {
            printf("Error connecting to device\n");
        }
        else
        {
            printf("Connected to device\n");
        }
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
            case '5':
                printf("Got 5\n");
                getInput(h);
                GenerateOutputMessage(echoBuffer);
            default:
                break;
        }

        sleep(5);
        info("closing..");
        libusb_close(h);
        delaying ();
        
        printf("sizeof(echoBuffer): %ld\n", (long)sizeof(echoBuffer));
        /* Echo message back to client */
        if (send (clntSocket, echoBuffer, RCVBUFSIZE, 0) != RCVBUFSIZE)
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

void GenerateOutputMessage(char* output)
{
    int stick_indexes[] = {6, 7, 8, 9, 10, 11, 12, 13};
    char** labels = {"LSX: ", "LSY: ", "RSX: ", "RSY: "};
    char tmp_string[8];

    output[0] = '\0';
    for (int i=0; i<2; i++)
    {
        for (int j=0; j<button_inputs[i]->number_of_bits; j++)
        {
            if ((BTN_INPUTS[button_inputs[i]->input_index] & (1<<button_inputs[i]->relevant_bits[j])) != 0)
            {
                strcat(output, button_inputs[i]->buttons[j]);
                printf("cat: '%s'\n", output);
            }
        }
    }
    

    strcat(output, " LT: ");
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[4]);
    strcat(output, tmp_string);

    strcat(output, " RT: ");
    tmp_string[0] = '\0';
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[5]);
    strcat(output, tmp_string);
    strcat(output, " ");

    for (int i=6; i<14; i++)
    {
    }
    

    strcat(output, "LSX: ");
    tmp_string[0] = '\0';
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[6]);
    strcat(output, tmp_string);
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[7]);
    strcat(output, tmp_string);
    strcat(output, " ");

    strcat(output, "LSY: ");
    tmp_string[0] = '\0';
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[8]);
    strcat(output, tmp_string);
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[9]);
    strcat(output, tmp_string);
    strcat(output, " ");

    strcat(output, "RSX: ");
    tmp_string[0] = '\0';
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[10]);
    strcat(output, tmp_string);
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[11]);
    strcat(output, tmp_string);
    strcat(output, " ");

    strcat(output, "RSY: ");
    tmp_string[0] = '\0';
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[12]);
    strcat(output, tmp_string);
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[13]);
    strcat(output, tmp_string);
    strcat(output, " ");

    printf("size of output: %ld\n", (long)sizeof(output));

}
