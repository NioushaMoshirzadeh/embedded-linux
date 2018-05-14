#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "delay.h"

/* Start and end of lowercase alphabet in ascii table */
#define LOWER_CASE_START 97
#define LOWER_CASE_END 122
/* File size 26 for each letter of the alphabet */
#define FILE_SIZE 26

static int shm_fd = -1;

/* fill array with lower case letters of the alphabet */
void createAlphabet(char* a);

/* Create new shared memory object returns address*/
char* my_shm_create(char* shm_name, int size);

int main()
{
    printf("Program A:\n");

    char* shm_addr = (char*) MAP_FAILED;
    char alphabet[FILE_SIZE]; 
    createAlphabet(alphabet);

    char* fileName = "/test";

    printf("Creating file..\n");
    shm_addr = my_shm_create(fileName, FILE_SIZE); 
    
    printf("Writing data to file..\n");
    for (int i=0; i<FILE_SIZE; i++)
    {
        shm_addr[i] = alphabet[i];
    }

    while (shm_addr[0] != 'A')
    {
        printf("Polling..\n");
        printf("\t found: %s\n", shm_addr);
        delay(1);
    }

    printf("Found 'A', closing file..\n");
    shm_unlink(fileName);

    printf("End program\n");
    return 0;
}

void createAlphabet(char* a)
{
    int index=0;
    for(int i=LOWER_CASE_START; i<=LOWER_CASE_END; i++)
    {
        a[index] = i;
        index++;
    }
}

char * my_shm_create(char * shm_name, int size)
{
    int     rtnval;
    char*   shm_addr;
    
    /*
     * create shared memory object
     * with shm_name as name
     * O_CREAT flag: create smo if it doesn't exist already
     * O_EXCL if the name already exists return an error
     * O_RDWR create with read-write access
     * Mode 0600 (r-w)
     * returns file decriptor if success else -1
     */
    shm_fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (shm_fd == -1)
    {
        perror ("ERROR: shm_open() failed");
    }
                
    /*
     * Set file size
     * if file was larger -> cut extra bytes
     * if file was smaller -> Fill new bytes with nullbytes
     * shm_fd: file descriptor
     * size: files size
     * returns 0 on success -1 on error
     */
    rtnval = ftruncate (shm_fd, size);
    if (rtnval != 0)
    {
        perror ("ERROR: ftruncate() failed");
    }
                
    /*
     * Create new mapping in virtual address space of calling process
     * NULL: address
     * size: length
     * r-w permissions
     * MAP_SHARED: updates are visible to other processes
     * shm_fd: file descriptor
     * 0: file offset
     * return starting address
     */
    shm_addr = (char *) mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }

    return (shm_addr);
}
