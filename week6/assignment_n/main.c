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
int numbers[2];
int number_index;
int tty_fd;                             // terminal file descriptor
//const char* sem_name = "ASSIGNMENT_N";  // semaphore file name
const char* sem_a_name = "SEM_A";
const char* sem_b_name = "SEM_B";
const char* sem_c_name = "SEM_C";
const char* sem_d_name = "SEM_D";
char* sem_name;

/*##############################################################*/

/*
 * Parse command line arguments, returns -1 on error
 * expected format is -p <program>
 * <program>: A, B, C or D
 */
int parseArgs(int argc, char** argv);
void initialize(void);

/*##############################################################*/

int main(int argc, char** argv)
{
    int rtnval;                 // return values from function calls
    int value = 0;              // semaphore value
    char buffer[5];             // hold number to print as string
    sem_t* sem_a = SEM_FAILED;    // semaphore
    sem_t* sem_b = SEM_FAILED;    // semaphore
    sem_t* sem_c = SEM_FAILED;    // semaphore
    sem_t* sem_d = SEM_FAILED;    // semaphore

    /* Parse arguments */
    rtnval = parseArgs(argc, argv);
    if (rtnval < 0)
    {
        exit(-1);
    }

    /* Give time for other programs to start */

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
        sem_a = sem_open(sem_a_name, O_CREAT | O_EXCL, 0600, 1);
        sem_b = sem_open(sem_b_name, O_CREAT | O_EXCL, 0600, 1);
        sem_c = sem_open(sem_c_name, O_CREAT | O_EXCL, 0600, 1);
        sem_d = sem_open(sem_d_name, O_CREAT | O_EXCL, 0600, 1);
    }
    else
    {
        sem_a = sem_open(sem_a_name, 0);
        sem_b = sem_open(sem_b_name, 0);
        sem_c = sem_open(sem_c_name, 0);
        sem_d = sem_open(sem_d_name, 0);
    }

    /* Print numbers to terminal with delay between numbers */
    for (int i=0; i<2; i++)
    {
        rtnval = sem_wait(sem_a);
        rtnval = sem_wait(sem_b);
        rtnval = sem_wait(sem_c);
        rtnval = sem_wait(sem_d);
        if (rtnval != 0)
        {
            printf("sem_wait() failed\n");
        }

        sprintf(buffer, "%d\n", numbers[number_index]);
        write(tty_fd, buffer, 2);
        number_index += 4;

        rtnval = sem_post(sem_d);
        rtnval = sem_post(sem_c);
        rtnval = sem_post(sem_b);
        rtnval = sem_post(sem_a);
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
        rtnval = sem_getvalue(sem_d, &value);
        if (rtnval != 0)
        {
            printf("sem_getvalue() failed\n");
        }
        sleep(1);
    }

    rtnval = sem_close(sem_d);
    rtnval = sem_close(sem_c);
    rtnval = sem_close(sem_b);
    rtnval = sem_close(sem_a);
    if (rtnval != 0)
    {
        printf("sem_close() failed\n");
    }

    rtnval = sem_unlink(sem_a_name);
    rtnval = sem_unlink(sem_b_name);
    rtnval = sem_unlink(sem_c_name);
    rtnval = sem_unlink(sem_d_name);
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
                        sem_name = (char *) sem_a_name;
                        break;
                    case 'B':
                        number_index = 1;
                        program = 1;
                        sem_name = (char *) sem_b_name;
                        break;
                    case 'C':
                        number_index = 2;
                        program = 2;
                        sem_name = (char *) sem_c_name;
                        break;
                    case 'D':
                        number_index = 3;
                        program = 3;
                        sem_name = (char *) sem_d_name;
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

void initialize()
{
    switch (program)
    {
        case 0:
            numbers = {1, 5};
            break;
        case 1:
            numbers = {2, 6};
            break;
        case 2:
            numbers = {3, 7};
            break;
        case 3:
            numbers = {4, 8};
            break;
    }
}
