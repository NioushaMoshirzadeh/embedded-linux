#include "xboxHandler.h"

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
    for (int i=0; i<len; i++)
    {
        printf("%d: %x, ", i, arr[i]);
    }
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
