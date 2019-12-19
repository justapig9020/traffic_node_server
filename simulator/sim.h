#ifndef _SIM_H_
#define _SIM_H_

#include "libsim.h"
#include "debug.h"
#include "method.h"

int sim_start(void);
int stop_sim(void);
int sim_update(void);
int chg_sig(int ,int);
int get_sig(int);
int get_cont(int, int);

#endif
