#include <stdio.h>      // printf
#include <stdlib.h>     // exit, atoi
#include <unistd.h>     // getopt, sleep
#include <fcntl.h>      // open, close
#include <semaphore.h>

/*##############################################################*/

#define TTY_PATH "/dev/pts/3"       // terminal

/*##############################################################*/

int program;                // only program 0 creates/deletes the file
const int numbers[8] = {1, 2, 3, 4, 5, 6, 7, 8}; // numbers to print
int number_index;           
int tty_fd;                             // terminal file descriptor
const char* sem_name = "ASSIGNMENT_N";  // semaphore file name

/*##############################################################*/

/*
 * Parse command line arguments, returns -1 on error
 * expected format is -p <program>
 * <program>: A, B, C or D
 */
int parseArgs(int argc, char** argv);

/*##############################################################*/

int main(int argc, char** argv)
{
    int rtnval;                 // return values from function calls
    int value = 0;              // semaphore value
    char buffer[5];             // hold number to print as string
    sem_t* sem = SEM_FAILED;    // semaphore

    /* Parse arguments */
    rtnval = parseArgs(argc, argv);
    if (rtnval < 0)
    {
        exit(-1);
    }

    /* Give time for other programs to start */
    sleep(5); 

    /* Setup terminal */
    tty_fd = open(TTY_PATH, O_RDWR);
    if (tty_fd < 0)
    {
        printf("Error finding terminal\n");
        exit(-1);
    }

    /* Create or open semaphore file */
    if (program == 0)
    {
        sem = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);
    }
    else
    {
        sem = sem_open(sem_name, 0);
    }

    /* Print numbers to terminal with delay between numbers */
    for (int i=0; i<2; i++)
    {
        rtnval = sem_wait(sem);
        if (rtnval != 0)
        {
            printf("sem_wait() failed\n");
        }

        sprintf(buffer, "%d\n", numbers[number_index]); 
        write(tty_fd, buffer, 2);
        number_index += 4;

        rtnval = sem_post(sem);
        if (rtnval != 0)
        {
            printf("sem_post() failed\n");
        }
        sleep(5);
    }

    /* if it's not the main program just end */
    if (program != 0)
    {
        close(tty_fd);
        exit(0);
    }

    /* main program waits till semaphore other programs finish then ends */
    while (value != 1)
    {
        rtnval = sem_getvalue(sem, &value);
        if (rtnval != 0)
        {
            printf("sem_getvalue() failed\n");
        }
        sleep(1);
    }

    rtnval = sem_close(sem);
    if (rtnval != 0)
    {
        printf("sem_close() failed\n");
    }

    rtnval = sem_unlink(sem_name);
    if (rtnval != 0)
    {
        printf("sem_unlink() failed\n");
    }

    close(tty_fd);

    return 0;

}

/*##############################################################*/

int parseArgs(int argc, char** argv)
{
    int opt;

    if (argc < 2)
    {
        printf("Missing required argument -p\n");
        return -1;
    }

    while((opt = getopt(argc, argv, "p:")) != -1)
    {
        switch(opt)
        {
            case 'p':
                switch (optarg[0])
                {
                    case 'A':
                        number_index = 0;
                        program = 0;
                        break;
                    case 'B':
                        number_index = 1;
                        program = 1;
                        break;
                    case 'C':
                        number_index = 2;
                        program = 2;
                        break;
                    case 'D':
                        number_index = 3;
                        program = 3;
                        break;
                    default:
                        printf("invalid argument\n");
                        return -1;
                        break;
                }
                break;
            case '?':
                return -1;
                break;
            default:
                return -1;
                break;
        }
    }

    return 0;
}
