#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *  print --help message
 */
void printHelp();

int main(int argc, char* argv[])
{
    int count_m = 0; // Track number of morning messages
    int count_e = 0; // Track number of evening messages

    /* print all arguments */
    printf("Arguments: ");
    for (int i=1; i<argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");

    /* 
     *   if --help command 
     *   print help message then return
     */
    if (strcmp(argv[1], "--help") == 0)
    {
        printHelp();
        return 0;
    }

    /*
     * check arguments 
     * -m or -e means single entry
     * -m [n] or -e [n] means n entries
     * save count to count_m, count_e
     */
    for (int i=1; i<argc; i+=2)
    {
        if (strcmp(argv[i], "-m") == 0)
        {
            count_m += i == argc-1 ? 1: atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-e") == 0) // if -e
        {
            count_e += i == argc-1 ? 1: atoi(argv[i+1]);
        }
    }

    /*
     * print morning and evening messages count_m and count_e times
     */
    for (int i=0; i<count_m; i++)
    {
        printf("Good morning\n");
    }
    for (int i=0; i<count_e; i++)
    {
        printf("Good evening\n");
    }

    return 0;
}

void printHelp()
{
    printf("helloArgv\n");
    printf("Arguments:\n");
    printf("  -m\t\tprint morning\n");
    printf("  -e\t\tprint evening\n");
    printf("  -m [n]\tprint morning [n] times\n");
    printf("  -e [n]\tprint evening [n] times\n");
    printf("  --help\tshow help\n");
}
