#include <stdio.h>

#include "sim.h"

int main(void)
{
    char c;

    sim_start ();
    chg_sig (0, 0);
    chg_sig (1, 1);
    chg_sig (2, 0);
    chg_sig (3, 0);
    chg_sig (4, 0);
    chg_sig (5, 0);
    chg_sig (6, 1);
    chg_sig (7, 0);
    while (1) {
        c = getchar();
        if (c == 'E')
            break;
        sim_update ();
        sim_show_sm ();
        sim_show_rt ();
    }
    sim_stop ();
    return 0;
}
