#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>

/*#####################################################################*/

#define TTY_PATH "/dev/pts/2"

/*#####################################################################*/

const char* SEM_ONE_NAME = "sem1";
const char* SEM_TWO_NAME = "sem2";

/*#####################################################################*/

void info(char* str);
sem_t* openOrCreate(const char* name);

/*#####################################################################*/

int main()
{
    sem_t*   sem_one = SEM_FAILED;
    sem_t*   sem_two = SEM_FAILED;

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

    sem_wait(sem_one);
    info("statment 1");
    sleep(3);
    sem_post(sem_one);

    sleep(3);

    sem_wait(sem_two);
    sem_wait(sem_one);
    info("statement 2");
    sleep(3);
    sem_post(sem_one);
    sem_post(sem_two);

    sleep(5);
    return 0;
}

/*#####################################################################*/

void wait(sem_t* sem)
{
    int rtnval;

    rtnval = sem_wait(sem);
    if (rtnval != 0)
    {
        perror("ERROR: sem_wait() failed");
    }
}

void post(sem_t* sem)
{
    int rtnval;

    rtnval = sem_post(sem);
    if (rtnval != 0)
    {
        perror("ERROR: sem_post() failed");
    }
}

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
