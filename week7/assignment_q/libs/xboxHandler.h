#ifndef _XBOXHANDLER_H_
#define _XBOXHANDLER_H_

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <errno.h>

#define VID     0x045e
#define PID     0x028e
#define INPUT   0x81
#define OUTPUT  0x02

extern unsigned char BTN_INPUTS[34];
extern unsigned char RUMBLE_ON[8];
extern unsigned char RUMBLE_OFF[8]; 
extern unsigned char LED_ALL_ON[3];
extern unsigned char LED_ALL_OFF[3];

libusb_device_handle* openDevice(void);
int sendCommand(libusb_device_handle* h, unsigned char* arr, int len);
void getInput(libusb_device_handle* h);

#endif
