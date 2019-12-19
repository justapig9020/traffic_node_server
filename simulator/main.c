#include <stdio.h>
#include <stdlib.h>

#include "libsim.h"
#include "method.h"


void show(struct simu *sm)
{
    printf ("\n=======\n");
    show_eg (&(sm->nd[0].eg[0]));
    puts ("");
    show_nd (&(sm->nd[2]));
    puts ("");
    show_eg (&(sm->nd[4].eg[1]));
}

int main(void)
{
    struct simu *sm;
    char c;
    sm = init_simu ();

    add_all_sig (sm, 0, fs0);
    add_all_sig (sm, 1, fs1);
    
    set_cr_gnr (sm, rand_gene);

    show_sm_conf (sm);
    
    system ("clear");
    while ((c = getchar()) != 'E') {
        update (sm);
        show (sm);
    }
    
    free_simu (sm);
    return 0;
}
