#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 0
#include "method.h"
#include "debug.h"

int fs0(struct node *nd)
{
    return 0;
}

int fs1(struct node *nd)
{
    return 0;
}

struct car *rand_gene(struct simu *sm, struct node *nd, int n)
{   
    int d; // direction
    struct car *c;
    struct car **cTail;
    struct path **pTail;
    int test = 0;
    int idir;           // in direction

    if (nd->sig != n/2) // exit direction not green light
        return NULL;

    srand (time (0));

    c = NULL;
    cTail = &(c);
    idir = n;

    dbg_arg ("lunch %d cars\n", nd->eg[n].pr.p);
    for (int i=0; i<nd->eg[n].pr.p; i++) {
        struct node *nptr;

        nptr = nd;
        *cTail = malloc (sizeof(struct car));

        (*cTail)->onTm = 0;
        (*cTail)->next = NULL;
        (*cTail)->path = NULL;
        pTail = &((*cTail)->path);
        n = idir;
        dbg_arg ("in dir %d\n", n);
        do {
            do {
                d = (n + (rand() % 3) + 1) % 4;
            } while (nptr->eg[d].pr.cp < 1);
            dbg_arg ("dir %d, node %d\n", d, nptr->adj[d]);
            (*pTail) = malloc (sizeof(struct path));
            (*pTail)->n = d;
            (*pTail)->next = NULL;
            pTail = &((*pTail)->next);
            nptr = nptr->eg[d].from;
            n = get_oppo (n);
            //show_nd_conf (nptr);
        } while (nptr->type != N_EXIT);
        cTail = &((*cTail)->next);
        nptr = NULL;
    }
    cTail = NULL;
    pTail = NULL;
    return c;
}
