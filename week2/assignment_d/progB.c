#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "delay.h"

/*###################################################################*/

/* file descriptor */
int shm_fd = -1;

/*###################################################################*/

/* Open shared memory object for r-w  returns address*/
char* my_shm_open(char* shm_name);

/*###################################################################*/

int main()
{
    printf("Program B:\n");

    char* shm_addr = (char*) MAP_FAILED; // create empty addr
    char* filename = "/test";            // file name

    printf("Opening file..\n");
    shm_addr = my_shm_open(filename); // open file

    /*
     * Check if the first letter of the file is a lowercase a
     * if it is change it to an uppercase A
     * end program
     */
    do
    {
        printf("Polling..\n");
        printf("Found: %s\n", shm_addr);
        delay(1);
    }
    while (shm_addr[0] != 'a');

    printf("Found 'a', changing to 'A'\n");
    shm_addr[0] = 'A';

    close(shm_fd); 
    
    printf("Program end");
    return 0;
}

/*###################################################################*/

char* my_shm_open (char* shm_name)
{
    int     size; // size of file in bytes
    char*   shm_addr; // address of file
    
    /*
     * create shared memory object
     * with shm_name as name
     * O_RDWR create with read-write access
     * Mode 0600 (r-w)
     * returns file decriptor if success else -1
     */
    shm_fd = shm_open(shm_name, O_RDWR, 0600);
    if (shm_fd == -1)
    {
        perror ("ERROR: shm_open() failed");
    }
                
    /*
     * reposition the file offset of the file decriptor to offset
     * shm_d: file descriptor
     * 0: offset
     * SEEK_END:File offset is set to size of file + offset ( so with
     *          offset of 0 returns the file size.
     * returns the resulting file offset in bytes from start of file 
     * (in this  case size of the file)
     */
    size = lseek(shm_fd, 0, SEEK_END);
    printf ("lseek() returned %d\n", size);
                
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
