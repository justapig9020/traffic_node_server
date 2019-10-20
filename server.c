#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> // for bzero
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "server.h"
#include "shmctl.h"
#include "debug.h"

static int init_server();
static int server_op(const struct data);
static void server_active(int, struct shmpg *);


int init_server()
{
    dbg ("server initing");
    int sockFd;
    struct sockaddr_in serverInfo;
    sockFd = socket (AF_INET , SOCK_DGRAM , 0);
    if (sockFd == -1){
        perror ("Socket create failed:");
        return -1;
    }

    bzero (&serverInfo, sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = htonl (INADDR_ANY);
    serverInfo.sin_port = htons (SER_PORT);

    if (bind (sockFd, (struct sockaddr *)&serverInfo, sizeof (serverInfo)) == -1) {
        perror ("Socket bind failed:");
        return -1;
    }
    dbg ("server inited");
    return sockFd;
}

int server_op(const struct data d)
{
    return 0;
}

void server_active(int sd, struct shmpg *shm)
{
    dbg ("server active");
    struct sockaddr_in clt_addr;
    struct data d;
    socklen_t clt_len;
    

    clt_len = sizeof(struct sockaddr_in);

    while (1) {
        int i;

        bzero (&d, sizeof(struct data));
        recvfrom (sd, &d, sizeof(struct data), 0, (struct sockaddr *)&clt_addr, &clt_len);

        for (i=0; i<shm->adjNum; i++) {
            if (clt_addr.sin_addr.s_addr == shm->adj[i].ip)
                break;
        }

        if (i != shm->adjNum) {
            push_data (d, &(shm->adj[i]));
        } else {
            server_op (d);
            break;
        }
    }

    del_shmpg (shm);
    shm = NULL;
}

struct shmpg *exec_server()
{
    dbg ("exec server");
    struct shmpg *shm;
    FILE *fd;
    int n;
    int sd;
    pid_t id;

    fd = fopen ("./adj_table", "r");
    if (!fd) {
        perror ("adj_table open failed:");
        return (struct shmpg *)-1;
    }
    
    fscanf (fd, "%d", &n);
    printf ("Adj node number: %d\n", n);

    shm = creat_shmpg (SHM_KEY_BS, n);
    if (shm == (struct shmpg *)-1) {
        perror ("creat shared memory failed:");
        return (struct shmpg *) -1;       
    }

    for (int i=0; i<n; i++) {
        char c[16];
        printf ("node %d ", i);
        fscanf (fd, "%s", c);
        printf ("ip: %s\n", c);
        shm->adj[i].ip = inet_addr (c);
    }

    fclose (fd);
    fd = NULL;
    
    sd = init_server ();

    if (sd == -1) {
        return (struct shmpg *)-1;
    }

    id = fork ();
    if (id == -1) {
        perror ("Fork server process failed:");
        del_shmpg (shm);
        return (struct shmpg *)-1;
    }

    if (id == 0) {
        /*
           New process for packground server
        */ 
        server_active (sd, shm);
        shm = NULL;

        close (sd);
        exit (0);
    }

    printf ("Run server @ pid: %d\n", id);

    return shm;
}
