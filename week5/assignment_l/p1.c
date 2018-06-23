#include "shm.h"

int main()
{
    /* numbers to print */
    int numbers[3] = {2, 4, 6};
    char buffer[10];            // buffer for printing strings

    /* open or create shm file */
    connect();

    /* set terminal */
    tty_fd = open(tty_name, O_RDWR);

    /* print every number to terminal */
    for (int i=0; i<3; i++)
    {
        waitShm(numbers[i] - 1);
        sprintf(buffer, "%d\n", numbers[i]);
        write(tty_fd, buffer, 2);
        setShm(numbers[i]);
    }

    /* Close shared memory and terminal */
    close(shm_fd);
    close(tty_fd);

    disconnect();
    return 0;
}
