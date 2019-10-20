#ifndef _NSHM_H_
#define _NSHM_H_

#include <sys/types.h>
#include <netinet/in.h>
#include "syn.h"

#define SHM_KEY_BS (key_t)100
#define BIN_TYPES 1

/*
    Recycled structs will translate to bin
*/
struct bin {
    struct bin *next;
};

struct data {
    int rate;
    struct data *next;
};

/*
    Adjacent nodes info
*/ 
struct node {
    in_addr_t ip;
    struct rwlock lock;
    //struct data *data; // if shtmalloc practiced
    struct data data;
};

/*
   Shared memory pages
*/
struct shmpg {
    char alive;                     // 0 for died, 1 for alive
    key_t key;                      // The key of shared memory
    void *end;                      // Point to the last byte of the page
    struct shmpg *nextPg; 
    void *hPtr;                     // Point to the top of heap
    struct bin *chunk[BIN_TYPES];   // Chunks of recycled bins
    int adjNum;
    struct node adj[1];            // Array of adjacent nodes info
};


struct shmpg *creat_shmpg(key_t, int num);
struct shmpg *get_shmpg(key_t);
int push_data(const struct data, struct node *);
struct data pop_data(struct node *);
int del_shmpg(struct shmpg *);
int clr_del_shmpg(key_t);

/* 
   TODO
   shtmalloc();
   shtfree();
*/

#endif
