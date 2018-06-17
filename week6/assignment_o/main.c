#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>

/*#####################################################################*/

/* Terminal to output to */
#define TTY_PATH "/dev/pts/2"

/*#####################################################################*/

/* Semaphore names */
const char* SEM_ONE_NAME = "sem1";
const char* SEM_TWO_NAME = "sem2";

/*#####################################################################*/

/*
 * Take str input and printf a message with the PID and str
 */
void info(char* str);

/*
 * Open a semaphore if it exists, or create it if it doesn't
 */
sem_t* openOrCreate(const char* name);

/*#####################################################################*/

int main()
{
    sem_t*   sem_one = SEM_FAILED;
    sem_t*   sem_two = SEM_FAILED;

    /* Create/Open semaphores */
    sem_one = openOrCreate(SEM_ONE_NAME);
    sem_two = openOrCreate(SEM_TWO_NAME);
    if (sem_one == SEM_FAILED)
    {
        perror("sem 1 failed\n");
        exit(-1);
    }
    if (sem_two == SEM_FAILED)
    {
        info("sem 2 failed\n");
        exit(-1);
    }

    /* Print first statement when sem_one is available */
    sem_wait(sem_one);
    info("statment 1");
    sleep(3);
    sem_post(sem_one);

    sleep(3);

    /*
     * Print second statement when sem_two and sem_one is available
     * sem_one again because we want the other programs to be finished
     * with printing their first statement
     */
    sem_wait(sem_two);
    sem_wait(sem_one);
    info("statement 2");
    sleep(3);
    sem_post(sem_one);
    sem_post(sem_two);

    sleep(3);

    /*
     * Wait till all programs have finished with their statement 2
     * before ending the program
     */
    sem_wait(sem_two);
    sem_wait(sem_one);
    sem_post(sem_one);
    sem_post(sem_two);
    return 0;
}

/*#####################################################################*/

sem_t* openOrCreate(const char* name)
{
    sem_t* sem = SEM_FAILED;

    sem = sem_open(name, 0);
    if (sem == SEM_FAILED)
    {
        /* file doesn't exist */
        if (errno == ENOENT)
        {
            sem = sem_open(name, O_CREAT | O_EXCL, 0600, 1);
            if (sem == SEM_FAILED)
            {
                perror("ERROR: sem_open() failed to create");
            }
        }
        /* error opening file */
        else
        {
            perror( "ERROR: sem_open() failed to open");
        }
    }
    return sem;
}

void info(char* str)
{
    printf("(%d) %s\n", getpid(), str);
}
