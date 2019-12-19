#include <stdio.h>
#include <stdlib.h>

#include "libsim.h"
#include "method.h"


int main(void)
{
    struct simu *sm;
    char c;
    sm = init_simu ();

    add_all_sig (sm, 0, fs0);
    add_all_sig (sm, 1, fs1);
    
    set_cr_gnr (sm, rand_gene);

    show_sm_conf (sm);
    
    while ((c = getchar()) != 'E') {
        update (sm);
        show_sm(sm);
    }
    
    free_simu (sm);
    return 0;
}
