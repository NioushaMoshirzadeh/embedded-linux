#include "shm.h"

int main()
{
    int numbers[3] = {1, 3, 5};     // numbers to print
    char buffer[10];                // buffer for printing string

    /* Open or create shm file */
    connect();

    /* Set terminal */
    tty_fd = open(tty_name, O_RDWR);


    /* print every number to terminal */
    for (int i=0; i<3; i++)
    {
        claimShm();
        sprintf(buffer, "%d\n", numbers[i]);
        write(tty_fd, buffer, 2);
        releaseShm();
    }

    close(shm_fd);
    close(tty_fd);

    claimShm();
    disconnect();
    return 0;
}
