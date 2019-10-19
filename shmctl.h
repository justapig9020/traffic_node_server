#ifndef _NSHM_H_
#define _NSHM_H_

#include <sys/types.h>

#define SEM_KEY (key_t)100
#define BIN_TYPES 1

struct bin {
    struct bin *next;
};

struct data {
    int rate;
    struct data *next;
};

struct node {
    in_addr_t ip;
    struct rwlock lock;
    //struct data *data; // if shtmalloc practiced
    struct data data;
};

struct shmpg {
    key_t key;                      // The key of shared memory
    void *end;                      // Point to the last byte of the page
    struct shmpg *nextPg; 
    void *hPtr;                     // Point to the top of heap
    struct bin *chunk[BIN_TYPES];   // Chunks of recycled bins
    int adjNum;
    struct node *adj[1];            // Array of adjacent nodes info
};

struct shmpg *creat_shmpg(key_t, int num);
struct shmpg *get_shmpg(key_t);
int push_data(const struct, int);
struct data pop_data(int);
int del_shmpg(struct shmpg *);

#endif
