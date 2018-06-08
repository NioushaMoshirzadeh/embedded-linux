#include "shm.h"

char* createShm()
{
    int rtnval;         // truncate return value
    char* addr;         // shm_addr in function scope

    /* create file with execute, read, write permission */
    shm_fd = shm_open(filename, O_CREAT | O_EXCL | O_RDWR, 0600);
    if ( shm_fd == -1)
    {
        perror("Error: shm_open() failed");
    }

    /* Set file size */
    rtnval = ftruncate(shm_fd, FILE_SIZE);
    if (rtnval != 0)
    {
        perror("Error: ftruncate() failed");
    }

    /* map file into memory */
    addr = (char*) mmap(NULL, FILE_SIZE, PROT_READ|PROT_WRITE, 
                            MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("Error: mmap() failed");
    }

    return addr;
}

char* openShm ()
{
    int     size; // size of file in bytes
    char*   addr;
    
    shm_fd = shm_open(filename, O_RDWR, 0600);
    if (shm_fd == -1)
    {
        return "FILE NOT FOUND";
    }
                
    /* Find file size */
    size = lseek(shm_fd, 0, SEEK_END);
    printf ("lseek() returned %d\n", size);
                
    addr = (char *) mmap (NULL, size, PROT_READ | PROT_WRITE, 
                                MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }
    
    return (addr);
}

void claimShm()
{
    /* Wait till file is available */
    while (shm_addr[0] != 0)
    {
        sleep(1);
    }
    /* claim file */
    shm_addr[0] = 1;
}

void releaseShm()
{
    /* release file */
    shm_addr[0] = 0;
    sleep(5);
}

void connect()
{
    /* initialize */
    shm_fd      = -1; 
    tty_name    = "/dev/pts/2";
    tty_fd      = -1;
    shm_addr    = "FILE NOT FOUND";
    filename    = "assignmentd";

    printf("Trying to open file..\n");
    shm_addr = openShm(filename);
    printf("shm_addr: %s\n", shm_addr);
    if (strcmp(shm_addr, "FILE NOT FOUND") == 0)
    {
        printf("File not found, creating new\n");
        shm_addr = createShm(filename, FILE_SIZE);
        shm_addr[0] = 0;
    }
    else
    {
        printf("File opened\n");
    }
}

void disconnect()
{
    shm_unlink(filename);
}
