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
static int server_op(const struct contant);
static void server_active(int, struct shmpg *);
static int clr_server();

int init_server()
{
    dbg ("server initing");
    int sockFd;
    struct sockaddr_in serverInfo;
    sockFd = socket (AF_INET , SOCK_DGRAM , 0);
    if (sockFd == -1){
        perror ("Socket create failed");
        return -1;
    }

    bzero (&serverInfo, sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = htonl (INADDR_ANY);
    serverInfo.sin_port = htons (SER_PORT);

    if (bind (sockFd, (struct sockaddr *)&serverInfo, sizeof (serverInfo)) == -1) {
        perror ("Socket bind failed");
        return -1;
    }
    dbg ("server inited");
    return sockFd;
}

int server_op(const struct contant d)
{
    return 0;
}

void server_active(int sd, struct shmpg *shm)
{
    dbg ("server active");
    struct sockaddr_in clt_addr;
    struct contant d;
    socklen_t clt_len;
    

    clt_len = sizeof(struct sockaddr_in);

    while (1) {
        int i;

        bzero (&d, sizeof(struct contant));
        recvfrom (sd, &d, sizeof(struct contant), 0, (struct sockaddr *)&clt_addr, &clt_len);
        
        dbg_arg ("ip_in: %d\n", clt_addr.sin_addr.s_addr);
        dbg_arg ("ANY: %d\n", htonl (INADDR_ANY));
        if (clt_addr.sin_addr.s_addr == 16777343) { // Server contral operation
            int ret;

            ret = server_op (d);
            if (ret == 0) // Stop server
                break;
            continue;
        }
        for (i=0; i<shm->adjNum; i++) {
            dbg_arg ("ip %d: %d\n", i, shm->adj[i].ip);
            if (clt_addr.sin_addr.s_addr == shm->adj[i].ip)
                break;
        }

        dbg_arg ("i: %d\n", i);
        if (i != shm->adjNum) {
            push_data (d, &(shm->adj[i]));
        }
    }
}

int clr_server()
{
    struct contant d;
    int cltFd;
    struct sockaddr_in serInfo;

    bzero (&d, sizeof(struct contant));

    cltFd = socket (AF_INET, SOCK_DGRAM, 0);
    if (cltFd == -1) {
        perror ("Creat socket in clr_server:");
        return -1;
    }

    bzero (&serInfo, sizeof(struct sockaddr_in));
    serInfo.sin_family = AF_INET;
    serInfo.sin_port = htons (SER_PORT);

    serInfo.sin_addr.s_addr = htonl(INADDR_ANY);

    sendto (cltFd, &d, sizeof(struct data), 0, (struct sockaddr *)&serInfo, sizeof(struct sockaddr));

    return 0;
}

struct shmpg *exec_server()
{
    dbg ("exec server");
    struct shmpg *shm;
    FILE *fd;
    int n;
    int sd;
    char c[16];
    pid_t id;

    fd = fopen ("./adj_table", "r");
    if (!fd) {
        perror ("adj_table open failed:");
        return (struct shmpg *)-1;
    }
    
    fscanf (fd, "%d", &n); // get adj nodes number
    printf ("Adj node number: %d\n", n);
    while (1) {
        puts ("Create shmpg");
        shm = creat_shmpg (SHM_KEY_BS, n);

        if (shm == (struct shmpg *)-1) {
            perror ("creat shared memory failed:");
            puts ("Clear shmpg");
            clr_shmg_cont ();
        } else {
            break;
        }
    }

    // read adj nodes ip
    for (int i=0; i<n; i++) {
        bzero (c, sizeof(c));
        printf ("node %d ", i);
        fscanf (fd, "%s", c);
        printf ("ip: %s\n", c);
        shm->adj[i].ip = inet_addr (c);
    }

#if MONITOR
    bzero (c, sizeof(c));
    fscanf (fd, "%s", c);
    shm->mip = inet_addr (c);
#endif

    fclose (fd);
    fd = NULL;
    
    while (1) {
        puts ("Init server");
        sd = init_server ();

        if (sd == -1) {
            int ret = 0;

            puts ("Clear server");
            ret = clr_server ();
            dbg_arg ("clr_server ret: %d\n", ret);

            usleep (1);
        } else {
            break;
        }
    }

    id = fork ();
    if (id == -1) {
        perror ("Fork server process failed:");
        if (del_shmpg (shm) == -1) {
            dbg ("del shmpg failed");
        }
        return (struct shmpg *)-1;
    }

    if (id == 0) {
        /*
           New process for background server
        */ 
        server_active (sd, shm);
        shm = NULL;

        if (close (sd) == -1) {
            perror ("close server failed:");
        }
        sd = -1;
        del_shmpg (shm);
        shm = NULL;

        exit (0);
    }

    printf ("Run server @ pid: %d\n", id);

    if (close (sd) == -1) {
        perror ("close server failed:");
    }
    sd = -1;

    return shm;
}
