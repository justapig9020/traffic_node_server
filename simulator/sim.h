#ifndef _SIM_H_
#define _SIM_H_

#include "libsim.h"
#include "debug.h"
#include "method.h"

int start_sim(int);
int sim_update();
int get_cont(int, int);
int chg_sig(int ,int);
int get_sig(int);
int stop_sim();

#endif
