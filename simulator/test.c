#include <stdio.h>

#include "err.h"

void f(void)
{
    cerrm ();
    serrm ("err1");
    serrm ("err2");
}

int main(void)
{
    perrm ("perrm 1");
    f ();
    perrm ("perrm 2");
    f ();
    perrm ("perrm3");
    return 0;
}
