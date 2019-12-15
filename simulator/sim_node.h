#ifndef _SIM_NODE_H_
#define _SIM_NODE_H_

struct edge
{
    int cpc;    // capacity (cars)
    int cont;   // content cars (cars)
    int flw;    // drive through intersection (cars / s)
    
    int tw;     // drive through edge
};

struct nd_pr    // node parameter
{
    int phase;  // light phase
    int stat;   // light status
    int exit;
    int *ip;
    struct edge *eg;
};

#endif
