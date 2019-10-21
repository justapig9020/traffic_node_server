#ifndef _SYN_H_
#define _SYN_H_

#include <sys/types.h>

#define SEM_KEY_BS (key_t)100
#define RL_KEY_BS (key_t)200
#define WL_KEY_BS (key_t)300
#define PW_KEY_BS (key_t)400

typedef int mut_t;

/*
 *  struct of r/w lock
 *  Read request not accept when writer waiting for write 
 *  until write process terminatdd 
 */
struct rwlock {
    int rNum;       // Number of reader
    mut_t rlock;
    mut_t wlock;
    mut_t pwait;    // Writer(producer) is waiting
};

/*
 *  mutex lock operations
 */
mut_t creat_mut (key_t);
mut_t get_mut (key_t);
int mut_lock (mut_t); 
int mut_rel (mut_t); 
int del_mut (mut_t);

/*
 *  r/w lock operations
 */
int init_rwlock (struct rwlock*, key_t, key_t, key_t);
int rw_rd (struct rwlock*);
int rw_wrt (struct rwlock*);
int rw_rd_end (struct rwlock*);
int rw_wrt_end (struct rwlock*);
int del_rwlock (struct rwlock*);

#endif
