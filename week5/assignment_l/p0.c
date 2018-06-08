#include "shm.h"

int main()
{
    int numbers[3] = {1, 3, 5};

    connect();

    tty_fd = open(tty_name, O_RDWR);


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

    claimShm();
    disconnect();
    return 0;
}
