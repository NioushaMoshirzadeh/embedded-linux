#ifndef _XBOXHANDLER_H_
#define _XBOXHANDLER_H_

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <errno.h>

/*######################################################################*/

#define VID     0x045e
#define PID     0x028e
#define INPUT   0x81
#define OUTPUT  0x02

/*######################################################################*/

extern unsigned char BTN_INPUTS[];
extern unsigned char RUMBLE_ON[];
extern unsigned char RUMBLE_OFF[];
extern unsigned char LED_ALL_ON[];
extern unsigned char LED_ALL_OFF[];

/*######################################################################*/

/*
 * Open the xbox device
 */
libusb_device_handle* openDevice(void);

/*
 * Send a commend to the deveice
 */
int sendCommand(libusb_device_handle* h, unsigned char* arr, int len);

/*
 * Get pressed button data from the device
 */
void getInput(libusb_device_handle* h);

/*######################################################################*/

typedef struct {
    int input_index;        // index of this byte in the input array
    int number_of_bits;     // number of relevant bits
    int relevant_bits[8];   // positions of the relevant bits
    char* buttons[8];       // Button names corresponding to relevant_bits by index
} INPUT_STRUCT;

extern INPUT_STRUCT input_two;
extern INPUT_STRUCT input_three;

/*
 * Array for easy iteration over all buttons
 */
extern INPUT_STRUCT* button_inputs[2];

/*######################################################################*/

#endif
