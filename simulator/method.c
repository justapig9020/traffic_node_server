#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 1
#include "method.h"
#include "debug.h"


static int c_id = 0;

/* 
   car cr enter next edge eg, cBuf use to buffer the next car of cr
    return 1 for sucess, 0 for fail
*/
int enter(struct car *cr, struct edge *eg, struct car **cBuf)
{
    int ret;

    dbg ("leaving");
    ret = 0;
    if (!eg) { // car exit the system
        ret = 1;
        *cBuf = cr->next;
        free (cr);
        cr = NULL;
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
    } 

    return ret;
}

/*
    car cr leave an edge eg, and update eg's parameter
*/
int leave(struct car *cr, struct edge *eg)
{
    dbg ("leaved\n");
    eg->cr = cr;
    eg->cont--;
    return 0;
}

/*
    signal of each edge turns green with it's opposite edge
*/
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

        to = NULL; // to = NULL mean this is the last node
        if (nd->eg[d].cr->path) {
            toD = nd->eg[d].cr->path->n;
            fromD = get_oppo (toD);
            dbg_arg ("%d %d\n", toD, fromD);
            to = &(nd->eg[toD].from->eg[fromD]);
            dbg_arg ("from %d, to %d\n", nd->num, nd->eg[toD].from->num);
        } else {
            dbg ("exit");
        }
    

        if (enter (nd->eg[d].cr, to, &cBuf)) {
            leave (cBuf, &(nd->eg[d]));
        } else {
            dbg ("leave failed\n");
            return i;
        }
        
        i++;
    }
    return i;
}

int fs0(struct node *nd)
{
    dbg_arg ("node %d fs %d\n", nd->num, nd->sig);
    for (int i=0; i<2; i++) { // phase 0: edge 0, 1
        if (nd->eg[i].pr.cp == 0) // no edge
            continue;
        strt (nd, i);
    }
    return 0;
}

int fs1(struct node *nd)
{
    dbg_arg ("node %d fs %d\n", nd->num, nd->sig);
    for (int i=2; i<4; i++) { // phase 1: edge 2, 3
        if (nd->eg[i].pr.cp == 0) // no edge
            continue;
        strt (nd, i);
    }
    return 0;
}


/*
    force car exit the system when it reach the terminal node
*/
int frs_out(struct node *nd)
{
    dbg_arg ("node %d fs %d force out\n", nd->num, nd->sig);
    for (int i=0; i<nd->egNum; i++) {
        if (nd->eg[i].pr.cp == 0 || nd->eg[i].pr.cp == -1)
            continue;
        strt (nd,i);
    }
    return 0;
}

struct car *rand_gene(struct simu *sm, struct node *nd, int n)
{   
    int d; // direction
    struct car *c;
    struct car **cTail;
    struct path **pTail;
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
        (*cTail)->id = 'A' + c_id;
        c_id = (c_id+1)%26;

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
