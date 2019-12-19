#include <stdio.h>

#include "sim.h"

int main(void)
{
    char c;

    sim_start ();
    while (1) {
        c = getchar();
        if (c == 'E')
            break;
        sim_update ();
        sim_show_sm ();
    }
    sim_stop ();
    return 0;
}
