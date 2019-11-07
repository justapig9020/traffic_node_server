#include <stdio.h>

#include "shmctl.h"
#include "syn.h"

int main(void)
{
    struct shmpg *shm;

    puts ("Del shmpg");
    if (clr_shmpg (SHM_KEY_BS) == -1) {
        perror ("del shmpg:");
    }
 
    puts ("Del RL");
    if (del_mut (RL_KEY_BS) == -1) {
        perror ("del rl mut");
    }

    puts ("Del WL");
    if (del_mut (WL_KEY_BS) == -1) {
        perror ("del wl mut");
    }

    puts ("Del PW");
    if (del_mut (PW_KEY_BS) == -1) {
        perror ("del pw mut");
    }
    return 0;
}
