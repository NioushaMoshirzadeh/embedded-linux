#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <mqueue.h>

typedef struct
{
    // a data structure with 3 members
    int                     a;
    int                     b;
    int                     c;
} MQ_REQUEST_MESSAGE;

static void 
getattr (mqd_t mq_fd)
{
    struct mq_attr      attr;
    int                 rtnval;
    
    rtnval = mq_getattr (mq_fd, &attr);
    if (rtnval == -1)
    {
        perror ("mq_getattr() failed");
    }
    fprintf (stderr, "%d: mqdes=%d max=%ld size=%ld nrof=%ld\n",
                getpid(), 
                mq_fd, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
}


int main(void)
{
    char        line[80];
    char        mq_name[80] = "/_mq_mq_";
    int         choice = ' ';
    int         rtnval = -1;
    mqd_t               mq_fd = -1;
    struct mq_attr      attr;
    MQ_REQUEST_MESSAGE  req;
    
    req.a = 80;
    req.b = 800;
    req.c = 8000;

    while (choice != 'q')
    {
        printf ("\nEnter choice (n,o,p,r,w,c,u,h=help,q): ");
        fgets (line, sizeof (line), stdin);
        choice = line[0];
        printf ("\n");
        
        switch (choice)
        {
            case 'n':
                attr.mq_maxmsg  = 3;
                attr.mq_msgsize = sizeof (MQ_REQUEST_MESSAGE);
                rtnval = mq_fd = mq_open (mq_name, O_WRONLY | O_CREAT | O_EXCL, 0600, &attr);
                break;
            case 'o':
                mq_fd = mq_open (mq_name, O_WRONLY);
                break;
            case 'p':
                mq_fd = mq_open (mq_name, O_RDONLY);
                break;
            case 'w':
                req.a++;
                req.b++;
                req.c++;
                rtnval = mq_send (mq_fd, (char *) &req, sizeof (req), 0);
                break; 
            case 'r':
                rtnval = mq_receive (mq_fd, (char *) &req, sizeof (req), NULL);
                printf ("MSG: a=%d b=%d c=%d\n", req.a, req.b, req.c);
                break;
            case 'c':
                rtnval = mq_close (mq_fd);
                break;
            case 'u':
                rtnval = mq_unlink (mq_name);
                break;
            case 'q':
                break;
            case 'h':
            case '?':
                printf ("Options:\n"
                        "========\n"
                        "    [n]  create new mq\n"
                        "    [o]  open existing mq (writing)\n"
                        "    [p]  open existing mq (reading)\n"
                        "    [w]  write\n"
                        "    [r]  read\n"
                        "    [c]  close\n"
                        "    [u]  unlink\n"
                        "    [h]  help\n"
                        "    [q]  quit\n");
                break;
            default:
                printf ("Invalid option '%c' (%#x)\n", choice, choice);
                break;
        }
        fprintf (stderr, "rtnval=%d fd=%d\n", rtnval, mq_fd);
        if (rtnval == -1)
        {
                perror ("--");
        }
        getattr(mq_fd);

    }
	return (0);
}
