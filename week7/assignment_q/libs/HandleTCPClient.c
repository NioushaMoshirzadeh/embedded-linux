#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <unistd.h>
#include <semaphore.h>
#include "xboxHandler.h"
#include "Auxiliary.h"
#include "HandleTCPClient.h"

/*######################################################################*/

#define RCVBUFSIZE 256  /* Size of receive buffer */

/*######################################################################*/
/*
 * Generate message showing the buttons pressed on the xbox controller
 */
void GenerateOutputMessage();
/*
 * Create message showing the value of the xbox controller sticks
 */
void createStickMessage(char* output, char* tmp_string, int msb_index, int lsb_index, char* label);

/*######################################################################*/

void HandleTCPClient (int clntSocket, sem_t* sem)
{
    char echoBuffer[RCVBUFSIZE ];       /* Message */
    int  recvMsgSize;                   /* Size of received message */
    libusb_device_handle* h = NULL;     /* Xbox controller */

    /* Receive message from client */
    recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
    if (recvMsgSize < 0)
    {
        DieWithError ("recv() failed");
    }

    /*
     * Main loop
     */
    while (recvMsgSize > 0)
    {
        info("Received message: %s\n", echoBuffer);

        /*
         * open device if semaphore is available
         */
        sem_wait(sem);
        h = openDevice();
        if (h == NULL)
        {
            info("Error connecting to device");
        }
        else
        {
            info("Connected to device");
        }


        /*
         * Check command
         */
        switch (echoBuffer[0])
        {
            case '1':
                info("Rumble on command");
                sendCommand(h, RUMBLE_ON, 8);
                break;
            case '2':
                info("Rumble off command");
                sendCommand(h, RUMBLE_OFF, 8);;
                break;
            case '3':
                info("Led on command");
                sendCommand(h, LED_ALL_ON, 8);
                break;
            case '4':
                info("Led off command");
                sendCommand(h, LED_ALL_OFF, 8);
                break;
            case '5':
                info("Output command");
                getInput(h);
                GenerateOutputMessage(echoBuffer);
                break;
            default:
                info("Defaulting..");
                break;
        }

        /*
         * Sleep for demo purposes
         * So it's obvious that the program is using a semaphore
         */
        sleep(5);

        /*
         * Close device
         */
        libusb_close(h);
        delaying ();

        /* Return message to client */
        if (send (clntSocket, echoBuffer, RCVBUFSIZE, 0) != RCVBUFSIZE)
        {
            DieWithError ("send() failed");
        }

        /*
         * Release semaphore
         */
        sem_post(sem);

        /* Display transmitted string in verbose mode */
        info(echoBuffer);
        echoBuffer[0] = '\0';

        // receive next string
        recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0);
        if (recvMsgSize < 0)
        {
            DieWithError ("recv() failed");
        }
        info("recv size: %d\n", recvMsgSize);
    }

    close (clntSocket);    /* Close client socket */
    info ("close");
}

/*######################################################################*/

void GenerateOutputMessage(char* output)
{
    /* intermediate string */
    char tmp_string[8] = {'\0'};
    output[0] = '\0';               // clear output string

    /*
     * Add messages for all buttons that are binary pressed / released
     */
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

    /*
     * Add messages for all buttons that return a number value
     */
    strcat(output, " LT: ");
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[4]);
    strcat(output, tmp_string);

    strcat(output, " RT: ");
    tmp_string[0] = '\0';
    sprintf(tmp_string, "0x%02x ", BTN_INPUTS[5]);
    strcat(output, tmp_string);
    strcat(output, " ");

    createStickMessage(output, tmp_string, 7, 6, " LSX: ");
    createStickMessage(output, tmp_string, 9, 8, " LSY: ");
    createStickMessage(output, tmp_string, 11, 10, " RSX: ");
    createStickMessage(output, tmp_string, 13, 12, " RSY: ");
    strcat(output, "\0");
}

void createStickMessage(char* output, char* tmp_string, int msb_index, int lsb_index, char* label)
{
    strcat(output, label);
    tmp_string[0] = '\0';
    sprintf(tmp_string, "0x%02x", BTN_INPUTS[msb_index]);
    strcat(output, tmp_string);
    sprintf(tmp_string, "%02x", BTN_INPUTS[lsb_index]);
    strcat(output, tmp_string);
    strcat(output, " ");
}
