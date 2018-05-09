#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

static void
child_activities (void)
{
    int     i;
    
    printf ("%d CHILD START\n", getpid());

    // do the work
    for (i = 0; i < 5; i++)
    {
        sleep (1);
        printf ("%d CHILD LOOP i=%d\n", getpid(), i);
    }
    
    printf ("%d CHILD READY\n", getpid());
}

int
main (void)
{
    pid_t   processID;      /* Process ID from fork() */
    
    printf ("%d main  start\n", getpid());
    
    /* creating the threads */
    processID = fork();
    if (processID < 0)
    {
        // fatal error!
        perror("fork() failed");
        exit (1);
    }
    else
    {
        if (processID == 0)
        {
            // processID == 0: child process
            child_activities();

            printf ("%d CHILD EXIT\n", getpid());
            exit(0);        /* Child process terminates */
        }
        else
        {
            // processID > 0: main process
            printf ("%d main  waiting...\n", getpid());
            sleep (2);
        }
    }
    
    printf ("%d main  exit\n", getpid());
    return (0);
}
