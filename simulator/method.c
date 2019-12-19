#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 0
#include "method.h"
#include "debug.h"


struct car *enter(struct car *cr, struct edge *eg)
{
    struct car *cBuf;

    cBuf = NULL; // if enter sucess return ptr to next car

    if (eg->pr.cp == -1) {
        cBuf = cr->next;

        free (cr->path); // car exit simu sys
        free (cr);
    } else if (eg->cont < eg->pr.cp) {
        struct car **cptr;
        struct path *pbuf;

        cptr = &(eg->cr);
        while (*cptr) {     // find tail of queue
            cptr = &((*cptr)->next);
        }

        *cptr = cr;
        cr->onTm = eg->pr.tw;

        pbuf = cr->path; // get next path
        cr->path = cr->path->next;
        free (pbuf);    // free last path
        pbuf = NULL;

        cBuf = cr->next; // set return val
        cr->next = NULL;

        cptr = NULL;
    }
    return cBuf;
}

int leave(struct car *cr, struct edge *eg)
{
    eg->cr = cr;
    eg->cont--;
    return 0;
}

int strt(struct node *nd, int d)
{
    int toD;
    int i;
    struct car *cBuf;

    i=0;
    if (nd->eg[d].cr && nd->eg[d].cr->onTm==0 && i<nd->eg[d].pr.p) {
        if (cBuf = enter (nd->eg[d].cr, &(nd->eg[nd->eg[d].cr->path->n].from->eg[get_oppo(d)])))
            leave (cBuf, &(nd->eg[d]));
    }
}

int fs0(struct node *nd)
{
    for (int i=0; i<2; i++) {
        if (nd->eg[i].pr.cp == 0)
            continue;
        strt (nd, i);
    }
    return 0;
}

int fs1(struct node *nd)
{
    for (int i=2; i<3; i++) {
        if (nd->eg[i].pr.cp == 0)
            continue;
        strt (nd, i);
    }
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
