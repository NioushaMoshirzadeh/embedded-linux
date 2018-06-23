#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>   // shm functions
#include <sys/stat.h>   // mode constants
#include <sys/types.h>  // lseek
#include <fcntl.h>      // O mode constats
#include <unistd.h>     // tty

/*###################################################################*/

/* Size to allocate for shm file */
#define FILE_SIZE sizeof(uint8_t)

/*###################################################################*/

int shm_fd;             // shm file descriptor
const char* tty_name;   // terminal name
int tty_fd;             // terminal file descriptor
char* shm_addr;         // shm data
const char* filename;   // shm file name


/*###################################################################*/

/*
 * Create new shm file
 */
char* createShm();

/*
 * Open the shm file
 */
char* openShm();

/*
 * wait till shm has num value
 */
void waitShm(int num);

/*
 * write num to shm
 */
void setShm(int num);

/*
 * Check if the shm file exists
 * if it doesn't create a new one, if it does open it
 */
void connect();

/*
 * unlink the shm file
 */
void disconnect();

/*###################################################################*/
