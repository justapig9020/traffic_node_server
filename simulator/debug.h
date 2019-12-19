#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>

#ifndef DEBUG
    #define DEBUG 1
#endif

#if DEBUG
    #define dbg_arg(x,...) printf(x, __VA_ARGS__)
    #define dbg(x) puts(x)
#else
    #define dbg_arg(x,...) 
    #define dbg(x)
#endif

#endif
