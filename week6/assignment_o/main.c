#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>

/*#####################################################################*/

const char* SEM_ONE_NAME = "sem1";
const char* SEM_TWO_NAME = "sem2";

/*#####################################################################*/

void wait(sem_t* sem);
void post(sem_t* sem);
void openOrCreate(sem_t* sem);

/*#####################################################################*/

int main()
{
    sem_t   sem_one = SEM_FAILED;
    sem_t   sem_two = SEM_FAILED;
    sem_t*  sem_one_ptr = &sem_one;
    sem_t*  sem_two_ptr = &sem_two;

    openOrCreate(sem_one_ptr);
    openOrCreate(sem_two_ptr);

    printf("Files opened, waiting ..\n");
    sleep(3);
    printf("Continuing..\n");
    
    sem_wait(sem_one_ptr);
    printf("statement 1\n");
    sem_post(sem_one_ptr);

    sem_wait(sem_one_ptr);
    sem_post(sem_one_ptr);

    sem_wait(sem_two_ptr);
    printf("statement 2\n");
    sem_post(sem_two_ptr);
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

void openOrCreate(sem_t* sem)
{
    sem = sem_open(SEM_ONE_NAME, 0);
    if (sem == SEM_FAILED)
    {
        /* file doesn't exist */
        if (errno == ENOENT)
        {
            sem = sem_open(SEM_ONE_NAME, O_CREAT | O_EXCL, 0600, 1);
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
}
