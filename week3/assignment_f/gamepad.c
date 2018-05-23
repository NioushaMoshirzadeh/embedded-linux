#include <stdio.h>
#include <libusb-1.0/libusb.h>

int main()
{
    int return_status;
    libusb_context* xbox_controller;
    libusb_device** list;

    printf("Initializing libusb\n");
    return_status = libusb_init(&xbox_controller);
    if (return_status != LIBUSB_SUCCESS)
    {
        printf("Error initializing libusb\n");
        return -1;
    }
    printf("Libusb initialized\n");

    printf("Setting libusb debug level\n");
    libusb_set_debug(xbox_controller, LIBUSB_LOG_LEVEL_WARNING);
    printf("Set libusb debug level\n");

    printf("Getting device list\n");
    ssize_t number_of_devices =  libusb_get_device_list(xbox_controller, &list);
    long ndevices = (long) number_of_devices;
    printf("Got %ld devices\n", ndevices);


    return 0;
}
