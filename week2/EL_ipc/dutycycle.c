#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>

static bool     to_quit = false;
static int      user_busy;
static int      system_busy;
static int      sleeping;
static int      scheduling;

static long long
convert_time (struct timespec * ts)
{
    // convert 'ts' to microseconds
    long long   nsec;
    long long   sec;

    // ensure that all items in the expression are 'long long'
    nsec = ts->tv_nsec;
    sec  = ts->tv_sec;
    return (nsec / 1000LL + sec * 1000000LL);
}

static long long
get_time (void)
{
    // get the elapsed time (in microseconds)
    struct timespec ts;
    
    clock_gettime (CLOCK_REALTIME, &ts);
    return (convert_time (&ts));
}

static long long
get_cputime (void)
{
    // get the consumed CPU-time (of this process)
    struct timespec ts;
    
    clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &ts);
    return (convert_time (&ts));
}

static long long
get_consumed_cputime (void)
{
    // get percent of CPU-time that this process has consumed (since last call)
    static long long    prev_cpu = 0;
    static long long    prev_time = 0;
    long long           curr_cpu;
    long long           curr_time;
    long long           result;

    curr_time = get_time();
    curr_cpu  = get_cputime();
    
    result  = (curr_cpu - prev_cpu);
    result *= 100LL;  // to get percent
    result /= (curr_time - prev_time);
    
    prev_cpu  = curr_cpu;
    prev_time = curr_time;
    
    return (result);
}

static void
another_cycle (void)
{
    static long long    counter = 0;
    static long long    prev_time = 0;
    long long           curr_time;
    
    counter++;
    curr_time = get_time();
    if (curr_time >= prev_time + 1000000LL)
    {
        // each second...
        printf ("%lld%%  %lld\n", get_consumed_cputime(), counter);
        prev_time = curr_time;
        counter = 0;
    }
}

extern int
main (int argc, char * argv[])
{
    struct timespec     ts;
    int                 i;

    if (argc != 5)
    {
        fprintf (stderr, "Usage: %s <user-busy> <system-busy> <sleeping> <scheduling>\n",
                 argv [0]);
        return (1);
    }
    user_busy   = atoi(argv[1]);
    system_busy = atoi(argv[2]);
    sleeping    = atoi(argv[3]);
    scheduling  = atoi(argv[4]);
    printf ("user=%d, system=%d sleeping=%d scheduling=%d\n",  
        user_busy, system_busy, sleeping, scheduling);
    
    while (to_quit == false)
    {
        another_cycle();
        
        for (i = 0; i < user_busy; i++)
        {
            // just wasting CPU-time
        }
        for (i = 0; i < system_busy; i++)
        {
            // eating CPU-time while performing system calls
            clock_gettime(CLOCK_REALTIME, &ts);
        }
        if (sleeping > 0)
        {
            // sleeping (so: not eating any CPU-time)
            usleep(sleeping);
        }
        for (i = 0; i < scheduling; i++)
        {
            // let other processes do their work
            sched_yield();
        }
    }
    // NOT REACHED
	return (0);
}

