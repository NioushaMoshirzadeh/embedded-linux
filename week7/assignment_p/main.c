#include <stdio.h>
#include <pthread.h>

int x = 0;

void* count_100()
{
    int i;
    for (i=0; i<100; i++)
    {
        x = x + 1;
    }
    return NULL;
}

int main()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, count_100, NULL);
    count_100();
    pthread_join(thread_id, NULL);
    printf("x = %d\n", x);
    return 0;
}
