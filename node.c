#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>

#include "shmctl.h" 
#include "server.h"
#include "debug.h"

static struct shmpg *shm = NULL;
static int cltFd = -1;
static struct sockaddr_in serInfo;

int send_d(int n, const struct contant d)
{
    if (shm == NULL) {
        fprintf (stderr, "Server doesnt active\n");
        return -1;
    }
    if (cltFd == -1) {
        cltFd = socket (AF_INET, SOCK_DGRAM, 0);
        if (cltFd == -1) {
            perror ("Creat socket in send_d:");
            return -1;
        }

        bzero (&serInfo, sizeof(struct sockaddr_in));
        serInfo.sin_family = AF_INET;
        serInfo.sin_port = htons (SER_PORT);
    }
    if (n == -1) {
        serInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        serInfo.sin_addr.s_addr = shm->adj[n].ip;
    }
    sendto (cltFd, &d, sizeof(struct contant), 0, (struct sockaddr *)&serInfo, sizeof(struct sockaddr));
    return 0;
}

struct contant recv_d(int n)
{
    struct contant d;
    if (shm == NULL) {
        fprintf (stderr, "Server doesnt active\n");
        bzero (&d, sizeof(struct contant));
        return d;
    }
    d = pop_data (&(shm->adj[n]));
    return d;
}

int start_server()
{
    dbg ("starting server");
    shm = exec_server ();
    if (shm == (struct shmpg *) -1) {
        dbg ("start server failed");
        return -1;
    }
    dbg ("Server started");
    return 0;
}

int stop_server()
{
    pid_t id;
    int s;
    struct contant d;

    dbg ("stoping server");
    if (shm == NULL) {
        fprintf (stderr, "Server doesnt active\n");
        return -1;
    }
    bzero (&d,sizeof(struct contant));
    if (send_d (-1, d) == -1) {
        fprintf (stderr, "@ stop server\n");
        return -1;
    }
    dbg ("servers stoped");
    id = wait (&s);

    del_shmpg (shm);
    shm = NULL;
    return id;
}
