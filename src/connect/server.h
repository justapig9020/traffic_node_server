#ifndef _SERVER_H_
#define _SERVER_H_

#include "shmctl.h"
#define SER_PORT 1234 
#define ADJ_TABLE "adj_table"

struct shmpg *exec_server();
//int stop_server(); // via socket

#endif
