#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>


static void *
my_thread_a (void * arg)
{
    int     s;
    int     i;

    // misuse the 'void *' parameter for passing an integer...
    s = (int) arg;
    printf ("%lx thread-a start s=%d\n", pthread_self(), s);

    /* Guarantees that thread resources are deallocated upon return */
    if (pthread_detach (pthread_self ()) != 0)
    {
        perror ("pthread_detach(a)");
    }

    // do the work
    for (i = 0; i < 5; i++)
    {
        usleep (s);
        printf ("%lx thread-a loop  i=%d\n", pthread_self(), i);
    }
    
    printf ("%lx thread-a exit\n", pthread_self());
    return (NULL);
}


static void *
my_thread_b (void * arg)
{
    int     s;
    int     i;

    s = (int) arg;
    printf ("%lx THREAD-B START s=%d\n", pthread_self(), s);
    
    if (pthread_detach (pthread_self ()) != 0)
    {
        perror ("pthread_detach(b)");
    }
    
    for (i = 8; i > 0; i--)
    {
        usleep (s);
        printf ("%lx THREAD-B LOOP  i=%d\n", pthread_self(), i);
    }
    
    printf ("%lx THREAD-B EXIT\n", pthread_self());
    return (NULL);
}


int main (void)
{
    pthread_t   thread_a;
    pthread_t   thread_b;
    
    printf ("%lx main     start\n", pthread_self());
    
    /* creating the threads */
    // misuse the 'void *' parameter for passing an integer...
    if (pthread_create (&thread_a, NULL, my_thread_a, (void *) 300000) != 0)
    {
        perror ("pthread_create(a)");
    }
    if (pthread_create (&thread_b, NULL, my_thread_b, (void *) 200000) != 0)
    {
        perror ("pthread_create(b)");
    }
    
    printf ("%lx main     waiting...\n", pthread_self());
    sleep (5);
    
    printf ("%lx main     exit\n", pthread_self());
    return (0);
}
