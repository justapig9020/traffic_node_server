#ifndef _NODE_H_
#define _NODE_H_

#include "shmctl.h"

int send_d(int, const struct contant);
struct contant recv_d(int);
int set_t(char*);
int start_server();
int stop_server();

#endif
