#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>     // strcmp
#include <sys/mman.h>   // shm functions
#include <sys/stat.h>   // mode constants  
#include <fcntl.h>      // O mode constats 
#include <sys/types.h>  // lseek
#include <unistd.h>     // lseek

/*###################################################################*/

#define FLAG_SIZE sizeof(uint8_t)
#define TERM_SIZE sizeof(char[11])
#define FILE_SIZE FLAG_SIZE + TERM_SIZE

/*###################################################################*/

typedef struct
{
    char* terminal;
    uint8_t flag;
} SHARED_DATA;

/*###################################################################*/

int shm_fd = -1;
int tty_fd = -1;

/*###################################################################*/

char* createShm(char* shm_name, int size);
char* openShm(char* shm_name);

/*###################################################################*/

int main()
{
    char* tty_name;
    tty_name = ttyname(fileno(stdin));
    char* shm_addr = "FILE NOT FOUND";
    char* filename = "assignmentd";
    SHARED_DATA data = {"f", 0};

    printf("Trying to open file..\n");
    shm_addr = openShm(filename);
    printf("shm_addr: %s\n", shm_addr);
    if (strcmp(shm_addr, "FILE NOT FOUND") == 0)
    {
        printf("File not found, creating new\n");
        shm_addr = createShm(filename, FILE_SIZE);
        data.terminal = tty_name;
        data.flag = 0;
        shm_addr = (char *) &data;
        printf("C: %s\n", ((SHARED_DATA *)(char *)shm_addr)->terminal);
        printf("C: %d\n", ((SHARED_DATA *)(char *)shm_addr)->flag);
    }
    else
    {
        printf("File opened\n");
        data.terminal = ((SHARED_DATA *)(char *)shm_addr)->terminal;
        printf("1\n");
        data.flag = ((SHARED_DATA *)(char *)shm_addr)->flag;
        printf("2\n");
        printf("hello\n");
        //printf("%s\n", data.terminal);
        //printf("%d\n", data.flag);
    }
    printf("Opening terminal\n");
    printf("tm: %s\n", data.terminal);
    tty_fd = open(data.terminal, O_RDWR);
    printf("term: %d\n", tty_fd);

    write(tty_fd, "1\n", 2);
    data.flag = 1;
    shm_addr = (char *) &data;
    while (data.flag != 0)
    {
        data.flag = ((SHARED_DATA *)(char *)shm_addr)->flag;
        sleep(1);
    }
    write(tty_fd, "2\n", 2);
    data.flag = 1;
    shm_addr = (char *) &data;
    while (data.flag != 0)
    {
        data.flag = ((SHARED_DATA *)(char *)shm_addr)->flag;
        sleep(1);
    }
    write(tty_fd, "3\n", 2);
    data.flag = 1;
    shm_addr = (char *) &data;
    while (data.flag != 0)
    {
        data.flag = ((SHARED_DATA *)(char *)shm_addr)->flag;
        sleep(1);
    }

    close(shm_fd);
    close(tty_fd);
    return 0;
}

/*###################################################################*/

char* createShm(char* shm_name, int size)
{
    int rtnval;
    char* shm_addr;

    shm_fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if ( shm_fd == -1)
    {
        perror("Error: shm_open() failed");
    }

    rtnval = ftruncate(shm_fd, size);
    if (rtnval != 0)
    {
        perror("Error: ftruncate() failed");
    }

    shm_addr = (char*) mmap(NULL, size, PROT_READ|PROT_WRITE, 
                            MAP_SHARED, shm_fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror("Error: mmap() failed");
    }

    return shm_addr;
}

char* openShm (char* shm_name)
{
    int     size; // size of file in bytes
    char*   shm_addr; // address of file
    
    printf("shm_open\n");
    shm_fd = shm_open(shm_name, O_RDWR, 0600);
    printf("shm_fd: %d\n", shm_fd);
    if (shm_fd == -1)
    {
        return "FILE NOT FOUND";
    }
                
    size = lseek(shm_fd, 0, SEEK_END);
    printf ("lseek() returned %d\n", size);
                
    shm_addr = (char *) mmap (NULL, size, PROT_READ | PROT_WRITE, 
                                MAP_SHARED, shm_fd, 0);
    if (shm_addr == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed");
    }

    return (shm_addr);
}
