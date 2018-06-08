#include "shm.h"

int main()
{
    /* numbers to print */
    int numbers[3] = {2, 4, 6};

    /* open or create shm file */
    connect();

    /* set terminal */
    tty_fd = open(tty_name, O_RDWR);

    /* print every number to terminal */
    for (int i=0; i<3; i++)
    {
        claimShm();
        char buffer[10];
        sprintf(buffer, "%d\n", numbers[i]);
        write(tty_fd, buffer, 2);
        releaseShm();
    }

    close(shm_fd);
    close(tty_fd);

    disconnect();
    return 0;
}
