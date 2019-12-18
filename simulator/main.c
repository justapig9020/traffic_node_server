#include <stdio.h>
#include <stdlib.h>



int main(void)
{
    struct simu *sm;
    sm = init_simu ();
    show_sm (sm);
    return 0;
}
