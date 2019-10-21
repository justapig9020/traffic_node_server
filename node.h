#ifndef _NODE_H_
#define _NODE_H_

#include "shmctl.h"

int send_d(int, const struct data);
struct data recv_d(int);
int start_server();
int stop_server();

#endif
