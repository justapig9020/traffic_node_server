#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 0
#include "method.h"
#include "debug.h"


int enter(struct car *cr, struct edge *eg, struct car **cBuf)
{
    int ret;

    puts ("leaving");
    ret = 0;
    if (!eg) {
        ret = 1;
        *cBuf = cr->next;
        free (cr);
    } else if (eg->cont < eg->pr.cp) {
        struct car **cptr;
        struct path *pbuf;

        ret = 1;
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

        *cBuf = cr->next; // set return val
        cr->next = NULL;

        cptr = NULL;
        eg->cont++;
    } else {
        if (eg)
            printf ("%d %d\n", eg->cont, eg->pr.cp);
        else 
            puts ("no path");
    }
    return ret;
}

int leave(struct car *cr, struct edge *eg)
{
    printf ("leaved\n");
    eg->cr = cr;
    eg->cont--;
    return 0;
}

int strt(struct node *nd, int d)
{
    int toD;    // direction
    int fromD;
    int i;     // lunch car counter 
    struct car *cBuf;
    struct edge *to;

    i=0; 
    while (nd->eg[d].cr && // car exist
        nd->eg[d].cr->onTm==0 && // car arrived node
        i<nd->eg[d].pr.p) { //  p car per turn

        to = NULL;
        if (nd->eg[d].cr->path) {
            toD = nd->eg[d].cr->path->n;
            fromD = get_oppo (toD);
            printf ("%d %d\n", toD, fromD);
            to = &(nd->eg[toD].from->eg[fromD]);
            printf ("from %d, to %d\n", nd->num, nd->eg[toD].from->num);
        } else {
            puts ("exit");
        }
    

        if (enter (nd->eg[d].cr, to, &cBuf)) {
            leave (cBuf, &(nd->eg[d]));
        } else {
            printf ("leave failed\n");
            return i;
        }
        
        i++;
    }
    return i;
}

int fs0(struct node *nd)
{
    printf ("node %d fs %d\n", nd->num, nd->sig);
    for (int i=0; i<2; i++) { // phase 0: edge 0, 1
        if (nd->eg[i].pr.cp == 0) // no edge
            continue;
        strt (nd, i);
    }
    return 0;
}

int fs1(struct node *nd)
{
    printf ("node %d fs %d\n", nd->num, nd->sig);
    for (int i=2; i<4; i++) { // phase 1: edge 2, 3
        if (nd->eg[i].pr.cp == 0) // no edge
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
        } while (nptr->type != N_EXIT); // find path until eixt
        cTail = &((*cTail)->next);
        nptr = NULL;
    }
    cTail = NULL;
    pTail = NULL;
    return c;
}
