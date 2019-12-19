#include <stdio.h>
#include <stdlib.h>

#include "libsim.h"
#include "method.h"

struct simu *sm;

int chg_sig(int n, int s)
{
    return sm->nd[n].sig = s;
}

void show(struct simu *sm)
{
    printf ("\n=======\n");
    //show_eg (&(sm->nd[0].eg[0]));
    show_nd (&(sm->nd[1]));
    puts ("");
    show_nd (&(sm->nd[2]));
    puts ("");
    show_nd (&(sm->nd[3]));
    //show_eg (&(sm->nd[4].eg[1]));
}

int main(void)
{
    char c;
    int n;

    n = 0;
    sm = init_simu ();

    add_all_sig (sm, 0, fs0);
    add_all_sig (sm, 1, fs1);
    
    set_cr_gnr (sm, rand_gene);
    chg_sig (1, 1);
    chg_sig (3, 1);

    show_sm_conf (sm);
    
    while ((c = getchar()) != 'E') {
        switch (c) {
            case 'C':
                n = !n;
                chg_sig (2, n);
            break;
        }
        if (c == '\n')
            system ("clear");
        update (sm);
        //show (sm);
        show_sm (sm);
        puts ("");
    }
    
    free_simu (sm);
    return 0;
}
