#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include "gamepad.h"

/*###################################################################*/

int main(int argc, char *argv[])
{
	libusb_device_handle *h;        // usb device handle

	libusb_init(NULL);              // init libusb library

    /* attempt to open xbox controller connection , return on error */
	h = libusb_open_device_with_vid_pid(NULL, VID, PID);
	if (h == NULL) {
		fprintf(stderr, "Failed to open device\n");
		return (1);
	}

    /* 
     * main program loop
     */
    int error, transferred;
    int run = 1;
    while (run == 1)
    {
        error = libusb_interrupt_transfer(
            h, IN, INPUT, sizeof(INPUT), &transferred, 0
        );
        if (error != 0)
        {
            fprintf(stderr, "Transfer failed: %d\n", error);
            error = 0;
            continue;
        }
        ShowInput();
        HandleTrigger(h, &trig_rmbl, &RUMBLE);
        HandleTrigger(h, &trig_led, &LED);
    }

	return (0);
}

/*###################################################################*/

void HandleTrigger(libusb_device_handle* h, TRIGGER* trigger, 
SEQUENCE* seq)
{
    int error, transferred;
    /* button is pressed */
    if ((INPUT[trigger->index] & (1<<trigger->bit)) != 0) 
    {
        if (trigger->active == 0)   // button isnt flagged as pressed 
        {
            error = libusb_interrupt_transfer(
                h, OUT,
                seq->cmds[seq->i],seq->cmd_len,
                &transferred, 0
            );
            if (error != 0)
            {
                fprintf(stderr, "Transfer failed: %d\n", error);
                error = 0;
            }
            else
            {
                /* if no error flag button as pressed */
                trigger->active = 1;
                /* increment index */
                seq->i = (seq->i < seq->len) ? seq->i + 1 : 0;
            }
        }
    }
    else
    {
        trigger->active = 0;        // flag button as unpressed
    }
}

void ShowInput()
{
    printf("\e[1;1H\e[2J");     // clear CLI
    printf("Buttons:\n\t");

    for (int i=0; i<2; i++)
    {
        for (int j=0; j<button_inputs[i]->number_of_bits; j++)
        {
            if ((INPUT[button_inputs[i]->input_index] & (1<<button_inputs[i]->relevant_bits[j])) != 0)
            {
                printf("%s, ", button_inputs[i]->buttons[j]);
            }
        }
    }
    printf("\n");

    printf("Triggers & sticks\n");
    printf("\tLeft trigger: \t%d\n", INPUT[4]);
    printf("\tRight trigger: \t%d\n", INPUT[5]);
    printf("\tLeft stick x: \t%d\n", INPUT[6]);
    printf("\tLeft stick y: \t%d\n", INPUT[8]);
    printf("\tRight stick x: \t%d\n", INPUT[10]);
    printf("\tRight stick y: \t%d\n", INPUT[12]);
}
