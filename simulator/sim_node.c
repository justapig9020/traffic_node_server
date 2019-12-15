#include <stdio.h>
#include <stdlib.h>

#include "sim_conf.h"
#include "err.h"

int init_sim()
{

}

int start_sim()
{
    FILE *fd;

    clr_errm ();
    fd = fopen (CONF_TAB);
    if (fd == NULL) {
        
    }
}
