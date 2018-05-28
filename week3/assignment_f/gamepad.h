#define VID 0x045e  // Vendor ID
#define PID 0x028e  // Product ID
#define IN  0x81    // message type for INPUT
#define OUT 0x02    // message type for OUTPUT

/*###################################################################
                Reading input from the device 
###################################################################*/
/*
 * The command to read input from the device
 * The result is stored in the same array
 */
unsigned char INPUT[] = {0, 34, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*
 * Struct to map the bits of bytes corresponding to buttons 
 */
typedef struct {
    int input_index;        // index of this byte in the input array
    int number_of_bits;     // number of relevant bits
    int relevant_bits[8];   // positions of the relevant bits
    char* buttons[8];       // Button names corresponding to relevant_bits by index
} INPUT_STRUCT; 

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


/*###################################################################
        Triggering actions based on buttons pressed 
###################################################################*/
/*
 * Stuct for setting up triggers
 */
typedef struct {
    int index;              // index of byte in the input array
    int bit;                // bit for trigger in the input byte
    int active;             // only trigger once per press
} TRIGGER;

/* rumble is triggered on button A input[3] bit 4 */
TRIGGER trig_rmbl   = {3, 4, 0};
/* rumble is triggered on button B input[3] bit 5 */
TRIGGER trig_led    = {3, 5, 0};


/*###################################################################
            Commands to be executed in a sequences
###################################################################*/
/*
 * Commands
 */
unsigned char RUMBLE_HIGH[]     = { 0, 8, 0, 0xff, 0xff, 0, 0, 0 };
unsigned char RUMBLE_MID[]      = { 0, 8, 0, 0x88, 0x88, 0, 0, 0 };
unsigned char RUMBLE_OFF[]      = { 0, 8, 0, 0x00, 0x00, 0, 0, 0 };

unsigned char LED_ALL_OFF[]     = {0x01, 0x03, 0x00};
unsigned char LED_ALL_ON[]      = {0x01, 0x03, 0x01};
unsigned char LED_ON_ONE[]      = {0x01, 0x03, 0x06};
unsigned char LED_ON_TWO[]      = {0x01, 0x03, 0x07};
unsigned char LED_ON_THREE[]    = {0x01, 0x03, 0x08};
unsigned char LED_ON_FOUR[]     = {0x01, 0x03, 0x09};

/*
 * Struct to store sequences
 */
typedef struct
{
    int cmd_len;            // length of command
    int len;                // length of sequence
    int i;                  // index
    unsigned char* cmds[];  // sequence
} SEQUENCE;

SEQUENCE RUMBLE = {8, 1, 0, {RUMBLE_OFF, RUMBLE_MID, RUMBLE_HIGH}};
SEQUENCE LED    = {3, 5, 0, {LED_ALL_ON, LED_ON_ONE, LED_ON_TWO,
                    LED_ON_THREE, LED_ON_FOUR, LED_ALL_OFF}
};

/*###################################################################
                    Function declarations
###################################################################*/

/*
 * Checks inputs for any triggers, and executes sequence for trigger
 */
void HandleTrigger(libusb_device_handle* h, TRIGGER* trigger, SEQUENCE* seq);

/*
 * Print all buttons being pressed to the Cli
 */
void ShowInput();

/*
 * Print 2 byte values as seperate bytes (hexadecimal, lsb first)
 */
void Show2ByteOutput(char* msg, unsigned char* msb, unsigned char* lsb);

/*###################################################################*/
