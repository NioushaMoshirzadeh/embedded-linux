#include "xboxHandler.h"

unsigned char BTN_INPUTS[] = {0, 34, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char RUMBLE_ON[]   = {0, 8, 0, 0x88, 0x88, 0, 0, 0};
unsigned char RUMBLE_OFF[]  = {0, 8, 0, 0x00, 0x00, 0, 0, 0}; 
unsigned char LED_ALL_ON[]  = {0x01, 0x03, 0x01};
unsigned char LED_ALL_OFF[] = {0x01, 0x03, 0x00};

INPUT_STRUCT input_two = {
    2, 8, {0, 1, 2, 3, 4, 5, 6, 7},
    {"DPAD UP", "DPAD DOWN", "DPAD LEFT", "DPAF RIGHT", 
    "START", "BACK", "L stick", "R stick"}
};
INPUT_STRUCT input_three = {
    3, 7, {0, 1, 2, 4, 5, 6, 7}, 
    {"LB", "RB", "XBOX", "A", "B", "X", "Y"}
}; 

/*
 * Array for easy iteration over all buttons
 */
INPUT_STRUCT* button_inputs[2] = {&input_two, &input_three};

libusb_device_handle* openDevice()
{
    libusb_device_handle* h;
    libusb_init(NULL);

    h = libusb_open_device_with_vid_pid(NULL, VID, PID);
    if (h == NULL)
    {
        perror("ERROR: libusb_open_device_with_vid_pid() failed");
        return NULL;
    }

    return h;
}

int sendCommand(libusb_device_handle* h, unsigned char* arr, int len)
{
    printf("sendCommand\n");
    int error, transferred;
    error = libusb_interrupt_transfer(
        h, OUTPUT, arr, len, &transferred, 0
        ); 
    if (error != 0)
    {
        perror("ERROR: libusb_interrupt_transfer failed");
        return -1;
    }
    return 0;
}

void getInput(libusb_device_handle* h)
{
    int error, transferred;
    error = libusb_interrupt_transfer(
        h, INPUT, BTN_INPUTS, sizeof(BTN_INPUTS), &transferred, 0
        );
    if (error != 0)
    {
        perror("ERROR: getInput() failed");
    }
}
