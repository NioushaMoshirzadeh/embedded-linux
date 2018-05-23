#include <libusb-1.0/libusb.h>
#include <stdio.h>

unsigned char RUMBLE_HIGH[] = { 0, 8, 0, 0xff, 0xff, 0, 0, 0 };
unsigned char RUMBLE_MID[] = { 0, 8, 0, 0x88, 0x88, 0, 0, 0 };
unsigned char RUMBLE_LOW[] = { 0, 8, 0, 0x22, 0x22, 0, 0, 0 };
unsigned char RUMBLE_OFF[] = { 0, 8, 0, 0, 0, 0, 0, 0 };
unsigned char INPUT[256] = {0, 34, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int main(int argc, char *argv[])
{
	libusb_device_handle *h;


	libusb_init(NULL);
	h = libusb_open_device_with_vid_pid(NULL, 0x045e, 0x028e);
	if (h == NULL) {
		fprintf(stderr, "Failed to open device\n");
		return (1);
	}

    int error, transferred;
    int run = 1;
    while (run == 1)
    {
        int cmd;
        printf("Enter: ");
        scanf("%d", &cmd);
        switch(cmd)
        {
            case 0:
                error = libusb_interrupt_transfer(
                    h, 0x02, RUMBLE_OFF, sizeof RUMBLE_OFF, 
                    &transferred, 0
                );
                break;
            case 1:
                error = libusb_interrupt_transfer(
                    h, 0x02, RUMBLE_LOW, sizeof RUMBLE_LOW, 
                    &transferred, 0
                );
                break;
            case 2:
                error = libusb_interrupt_transfer(
                    h, 0x02, RUMBLE_MID, sizeof RUMBLE_MID, 
                    &transferred, 0
                );
                break;
            case 3:
                error = libusb_interrupt_transfer(
                    h, 0x02, RUMBLE_HIGH, sizeof RUMBLE_HIGH, 
                    &transferred, 0
                );
                break;
            case 4:
                error = libusb_interrupt_transfer(
                    h, 0x81, INPUT, sizeof(INPUT), &transferred, 0
                );
                printf("Transferred: %d\n", transferred);
                switch (INPUT[2])
                {
                    case 1:
                        printf("Up d-pad\n");
                        break;
                    case 2:
                        printf("Down d-pad\n");
                        break;
                    case 4:
                        printf("Left d-pad\n");
                        break;
                    case 8:
                        printf("Right d-pad\n");
                        break;
                }
                printf("\n");
                break;
            default:
                run = 0;
                break;
        }
        if (error != 0)
        {
            fprintf(stderr, "Transfer failed: %d\n", error);
            error = 0;
        }
        
    }

	return (0);
}
