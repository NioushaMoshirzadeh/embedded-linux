#include "delay.h"

void delay(int seconds)
{
    /* Get delaytime in terms of clock cycles per second */
    int delaytime = CLOCKS_PER_SEC * seconds;
    
    /* Get time now */
    clock_t starttime = clock();

    /* Loop untill the time is larger than target */
    while (clock() < starttime + delaytime);
}
