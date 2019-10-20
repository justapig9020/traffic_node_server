#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>

#include "debug.h"
#include "shmctl.h"
#include "syn.h"

static void *creat_shm(key_t);
static void *get_shm(key_t);
static int del_shm(key_t);

void *creat_shm (key_t key)
{
    dbg ("creat shm");
    int id;
    void *addr;
    int size;

    size = getpagesize ();

    id = shmget (key, size, 0644 | IPC_CREAT | IPC_EXCL);
    if (id == -1)
        return (void *)-1;

    addr = shmat (id, NULL, 0);
    bzero (addr, size);

    dbg ("success");
    return addr;
}

void *get_shm (key_t key)
{
    int id;
    int size;

    size = getpagesize ();

    id = shmget (key, size, 0644 | IPC_CREAT);
    if (id == -1)
        return (void *)-1;

    return shmat (id, NULL, 0);
}

int del_shm(key_t key)
{
    int shmid;
    int size;

    size = getpagesize ();

    shmid = shmget (key, size, 0644);
    if (shmid == -1)
        return -1;

    if (shmctl (shmid, IPC_RMID, 0) == -1)
        return -1;

    return 0;
}

struct shmpg *creat_shmpg(key_t key, int num)
{
    struct shmpg *shm;
    
    shm = (struct shmpg *)creat_shm (key);
    if (shm == (struct shmpg*)-1) {
        return (struct shmpg *)-1;
    }
    shm->alive = 0;
    shm->key = key;
    shm->end = (void *)((int)shm + getpagesize () - 1);
    shm->nextPg = NULL;
    shm->adjNum = num;

    for (int i=0; i<BIN_TYPES; i++) {
        shm->chunk[i] = NULL;
    }

    for (int i=0; i<num; i++) {
        init_rwlock (&(shm->adj[i].lock), RL_KEY_BS+i, WL_KEY_BS+i, PW_KEY_BS+i);
        //shm->adj[i].data = NULL; // if shtmalloc practiced
        shm->adj[i].data.rate = 0;
        shm->adj[i].data.next = NULL; 
    }

    shm->hPtr  = (void *)((int)(shm) + 
                        sizeof(struct shmpg) + 
                        sizeof(struct node)*(num-1));
    shm->alive = 1;

    return shm;
}

struct shmpg *get_shmpg(key_t key)
{
    struct shmpg *shm;

    shm = (struct shmpg *)get_shm (key);

    if (shm == (struct shmpg *)-1) {
        return (struct shmpg *) -1;
    }

    /*if (shm->alive == 0)
        return (struct shmpg *)-1;
    */
    return shm;
}

int push_data(const struct data d, struct node *target)
{
    rw_wrt (&(target->lock));
    target->data = d; // Need to modify after practiced shtmalloc
    rw_wrt_end (&(target->lock));
    return 0;
}

struct data pop_data(struct node* target)
{   
    struct data ret;

    rw_rd (&(target->lock));
    ret = target->data;
    rw_rd_end (&(target->lock));

    return ret;
}

int del_shmpg(struct shmpg *shm)
{
    key_t key;

    dbg ("del shmpg");
    shm->alive = 0;
    key = shm->key;
    if (shmdt (shm) == -1) {
        perror ("Detach shared memory failed:");
        return -1;
    }

    dbg ("dted shmpg");

    if (del_shm (key) == -1) {
        perror ("Delete shared memory failed:");
        return -1;
    }
    
    dbg ("deled shmpg");

    return 0;
}

int clr_del_shmpg(key_t key)
{
    struct shmpg *shm;
    shm = get_shmpg(key);

    dbg ("del shmpg");
    if (shmdt (shm) == -1) {
        perror ("Detach shared memory failed:");
        return -1;
    }

    dbg ("dted shmpg");

    if (del_shm (key) == -1) {
        perror ("Delete shared memory failed:");
        return -1;
    }
    
    dbg ("deled shmpg");

    return 0;
}
