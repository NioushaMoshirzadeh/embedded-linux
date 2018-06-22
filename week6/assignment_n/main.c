#include <stdio.h>      // printf
#include <stdlib.h>     // exit, atoi
#include <string.h>
#include <unistd.h>     // getopt, sleep
#include <fcntl.h>      // open, close
#include <semaphore.h>
#include <errno.h>

/*##############################################################*/

#define TTY_PATH "/dev/pts/3"       // terminal

/*##############################################################*/
int program;
int tty_fd;
char* sem_names[8] = {
    "SEM_1", "SEM_2", "SEM_3", "SEM_4",
    "SEM_5", "SEM_6", "SEM_7", "SEM_8"
};
/*
 * Each program uses 4 semaphores, 2 to let other programs know when
 * it's finished with a task and 2 to check whether another program
 * has finished its task
 * the indexes are (0, 1) for other and (2, 3) for own
 */
sem_t* sems[4] = {SEM_FAILED, SEM_FAILED, SEM_FAILED, SEM_FAILED};
int indexes[4];
char* messages[8] = {
    "1\n", "2\n", "3\n", "4\n",
    "5\n", "6\n", "7\n", "8\n"
};

/*##############################################################*/
void parseArgs(int argc, char** argv);
void setIndexes(void);
void createOrOpenSemaphores(void);
void wait(int action);
void post(int action);
void closeSemaphores(void);
void closeSemaphores(void);
void unlinkSemaphores(void);

/*##############################################################*/

int main(int argc, char** argv)
{
    // parse arguments
    printf("Parsing arguments...\n");
    parseArgs(argc, argv);

    // create/open semaphores
    printf("Setting indexes...\n");
    setIndexes();
    printf("Creating semaphores...\n");
    createOrOpenSemaphores();

    // connect tty
    printf("Connecting to terminal...\n");
    tty_fd = open(TTY_PATH, O_RDWR);

    //sleep(5);
    // wait and print numbers
    wait(1);
    write(tty_fd, messages[indexes[2]], strlen(messages[indexes[2]]));
    post(1);
    wait(2);
    write(tty_fd, messages[indexes[3]], strlen(messages[indexes[3]]));
    post(2);

    closeSemaphores();

    unlinkSemaphores();

    // disconnect tty
    close(tty_fd);
    sleep(5);
}

/*##############################################################*/

void parseArgs(int argc, char** argv)
{
    int opt;
    if (argc < 2)
    {
        perror("Missing required argument -p");
    }

    while ((opt = getopt(argc, argv, "p:")) != -1)
    {
        if (opt == 'p')
        {
            switch (optarg[0])
            {
                case 'A':
                    program = 0;
                    break;
                case 'B':
                    program = 1;
                    break;
                case 'C':
                    program = 2;
                    break;
                case 'D':
                    program = 3;
                    break;
                default:
                    perror("Invalid argument");
                    break;
            }
        }

    }
}

void setIndexes()
{
    switch (program)
    {
        case 0:
            indexes[0] = -1;
            indexes[1] = 3;
            indexes[2] = 0;
            indexes[3] = 4;
            break;
        case 1:
            indexes[0] = 0;
            indexes[1] = 4;
            indexes[2] = 1;
            indexes[3] = 5;
            break;
        case 2:
            indexes[0] = 1;
            indexes[1] = 5;
            indexes[2] = 2;
            indexes[3] = 6;
            break;
        case 3:
            indexes[0] = 2;
            indexes[1] = 6;
            indexes[2] = 3;
            indexes[3] = 7;
            break;
    }
}

void createOrOpenSemaphores()
{
    int i;
    for (int j=0; j<sizeof(indexes)/sizeof(int); j++)
    {
        i = indexes[j];
        if (i != -1)
        {
            sems[j] = sem_open(sem_names[i], O_CREAT|O_EXCL, 0600, 1);
            /* error 17 if file already exists */
            if (errno == 17)
            {
                sems[j] = sem_open(sem_names[i], 0);
                errno = 0;
            }
        }
    }
}

void wait(int action)
{
    if (action == 1)
    {
        sem_wait(sems[2]);
        sleep(5);
        if (sems[0] != SEM_FAILED)
        {
            sem_wait(sems[0]);
            sleep(3);
        }


    }
    else
    {
        sem_wait(sems[3]);
        sleep(3);
        sem_wait(sems[1]);
        sleep(3);
    }

}

void post(int action)
{
    if (action == 1)
    {
        sem_post(sems[2]);
        sleep(3);
        if (sems[0] != SEM_FAILED)
        {
            sem_post(sems[0]);
            sleep(3);
        }
    }
    else
    {
        sem_post(sems[3]);
        sleep(3);
        sem_post(sems[1]);
        sleep(3);
    }

}

void closeSemaphores(void)
{
    sem_close(sems[2]);
    sem_close(sems[3]);
}

void unlinkSemaphores(void)
{
    sem_unlink(sem_names[indexes[2]]);
    sem_unlink(sem_names[indexes[3]]);
}
