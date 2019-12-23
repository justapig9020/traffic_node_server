#ifndef _NSHM_H_
#define _NSHM_H_

#include <sys/types.h>
#include <netinet/in.h>
#include "syn.h"

#define SHM_KEY_BS (key_t)100
#define BIN_TYPES 1             // Number of bin types

/*
 *  Recycled structs will translate to bin
 */
struct bin {
    struct bin *next;
};

struct contant {
    char json[200];
};

struct data {
    struct contant contant;
    struct data *next;
};

/*
 *  Adjacent nodes info
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
    struct shmpg *nextPg;           // Point to the next shmpg if exist
    void *hPtr;                     // Point to the top of heap
    struct bin *chunk[BIN_TYPES];   // Chunks of recycled bins
#if MONITOR
    int mip;                        // Monitor ip
#endif
    int adjNum;                     // The number of adj nodes
    struct node adj[1];             // Array of adjacent nodes info
};

/* 
 *  Create a shared memory page
 *  Return value:
 *      On success return the pointer to the allocated memory address
 *      On error return -1 and errno is set appropriately
 */
struct shmpg *creat_shmpg(key_t, int num);

/*
 *  Get a shared memory page which has been create 
 *  Return value:
 *      On success return the pointer to the allocated memory address
 *      On error return -1 and errno is set appropriately
 */
struct shmpg *get_shmpg(key_t);

int clr_shmg_cont(void);
/*
 *  Push a data in to stack of specific adjacent node
 *  Return value:
 *      return 0;
 */ 
int push_data(const struct contant, struct node *);

/*
 *  Pop a data from the stack of specific adjacentnode
 *  Return value:
 *      data
 */ 
struct contant pop_data(struct node *);

/*
 *  Delete a shared memory page and its contant
 *  Return value:
 *      On success return 0
 *      On error print all erron and return -1
 */
int del_shmpg(struct shmpg *);

/* Only delete shared memory */
int clr_shmpg(key_t);

/* 
   TODO
   shtmalloc();
   shtfree();
*/

#endif
