/*
 * de volgende code laat het ledje op de gamepad rondspinnen:
*/

#include <libusb-1.0/libusb.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	libusb_device_handle *h;
	//unsigned char data[] = { 1, 3, 0x0a };
    unsigned char data[] = { 00, 8, 00, 99, 99, 00, 00, 00 };
	int error, transferred;

	libusb_init(NULL);
	h = libusb_open_device_with_vid_pid(NULL, 0x045e, 0x028e);
	if (h == NULL) {
		fprintf(stderr, "Failed to open device\n");
		return (1);
	}

	if ((error = libusb_interrupt_transfer(h, 0x02, data, sizeof data, &transferred, 0)) != 0) {
		fprintf(stderr, "Transfer failed: %d\n", error);
		return (1);
	}

	return (0);
}
