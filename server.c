#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "server.h"
#include "syn.h"

int server_init (char *ip, int num)
{
    char inBuf;
    int sockFd = 0;
    struct sockaddr_in serverInfo;
    sockFd = socket (AF_INET , SOCK_DGRAM , 0);
    inBuf = malloc (1024);
    if (sockFd == -1){
        perror ("Socket create failed:");
        return -1;
    }

    bzero (&serverInfo, sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = htonl (INADDR_ANY);
    serverInfo.sin_port = htons (SER_PORT);

    if (bind (sockfd, (struct sockaddr *)&serverInfo, sizeof (serverInfo)) == -1) {
        perror ("Socket bind failed:");
        return -1;
    }

    return sockFd;
}


static inline void

static inline void push_data (const struct data d, struct adj *node)
{
    rw_wrt (node->lock);
    node->data = d;
    rw_wrt_end (node->lock);
}

static void server_active(int sd, in_addr_t *ip, int num)
{
    struct shmpg *shm;
    struct sockaddr_in clt_addr;
    struct data d;
    int clt_len;
    
    shm = creat_shmpg (SHM_KEY, num);
    clt_len = sizeof(struct sockaddr_in);
    while (1) {
        int i;
        in_addr_t ipBuf;
        bzero (&d, sizeof(struct data));
        recvfrom (sd, &d, sizeof(struct data), 0, (struct sockaddr *)&clt_addr, &clt_len);
        for (i=0; i<num; i++) {
            if (clt_addr.sin_addr == ip[i])
                break;
        }
        if (i != num) {
            push_data (d, &(shm->node[i]));
        } else {
            server_operation (d);
        }
    }
    shmdt (shm);
    del_shm ()
}

int server_run(int sd)
{
    FILE *fd;
    pid_t id;
    int n;
    
    in_addr_t *ip;
    fd = fopen ("./adj_table", "r");
    if (!fd) { perror ("adj_table open failed:");
        return -1;
    }
    
    fscanf (fd, "%d", &n);
    ip = malloc (sizeof(in_addr_t)*n);
    for (int i=0; i<n; i++) {
        char c[16];
        scanf ("%s", c);
        ip[i] = inet_addr (c);
    }
    fclose (fd);
    fd = NULL;
    
    id = fork ();
    if (id == -1) {
        perror ("Fork server process failed:");
        free (ip);
        ip = NULL;
        return -1;
    }

    if (id == 0) {
        server_active (sd, ip, n);
    } else {
        printf ("Run server @ pid: %d\n", id);
    }
    return 0;
}
