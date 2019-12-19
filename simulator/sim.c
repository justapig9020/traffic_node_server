#include <stdio.h>
#include <stdlib.h>
#include "sim.h"

struct simu *sm;


int sim_start(void)
{
    int n;

    n = 0;
    sm = init_simu ();

    add_all_sig (sm, 0, fs0); // set phase 0 method
    add_all_sig (sm, 1, fs1); // set phase 1 method
    set_cr_gnr (sm, rand_gene); // set car generation function
    
    return 0;
}

int sim_stop(void)
{
    return free_simu (sm);
}

int sim_update(void)
{
    return update (sm);
}

int chg_sig(int n, int s)
{
    return (sm->nd[n].sig = s);
}

int get_sig(int n)
{
    return sm->nd[n].sig;
}

int get_cont(int n, int e)
{
    return sm->nd[n].eg[e].cont;
}
