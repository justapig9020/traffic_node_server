#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>

#define DEBUG 1

#if DEBUG
    #define dbg(x) puts(x)
#else
    #define dbg(x)
#endif

#endif
