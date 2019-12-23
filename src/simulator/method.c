#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 0
#include "method.h"
#include "debug.h"


#if DEBUG == 4
static int g_total = 0;
static int long_p = 0;
#endif

static int c_id = 0;

int enter(struct car *cr, struct edge *eg, struct car **cBuf)
{
    int ret;

    //dbg ("leaving");
    ret = 0;
    if (!eg) {
        ret = -1;
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
             dbg_arg_n (1, "%d %d\n", eg->cont, eg->pr.cp);
        //else 
            //dbg ("no path");
    }
    return ret;
}

int leave(struct car *cr, struct edge *eg)
{
    //dbg ("leaved\n");
    eg->cr = cr;
    eg->cont--;
    return 0;
}

static void update_rate (struct node *nd, int toD, int d)
{
    if (toD == get_oppo(d))
        nd->r.fw++;
    else if ((toD&1&d) != d/2)
        nd->r.lf++;
    else
        nd->r.rt++;
    nd->r.total++;
}

int strt(struct node *nd, int d)
{
    int toD;    // direction
    int fromD;
    int i;     // lunch car counter 
    int turn;
    struct car *cBuf;
    struct edge *to;
    int exit;

    i=0; 
    while (nd->eg[d].cr && // car exist
        nd->eg[d].cr->onTm==0 && // car arrived node
        i<nd->eg[d].pr.p) { //  p car per turn

        to = NULL;
        if (nd->eg[d].cr->path) {
            toD = nd->eg[d].cr->path->n;
            fromD = get_oppo (toD);
             dbg_arg_n (1, "%d %d\n", toD, fromD);
            to = &(nd->eg[toD].from->eg[fromD]);
             dbg_arg_n (1, "from %d, to %d\n", nd->num, nd->eg[toD].from->num);
        } else {
           // dbg ("exit");
        }
    
        if ((exit = enter (nd->eg[d].cr, to, &cBuf))) {
            if (exit != -1)
                update_rate(nd, toD, d);
            leave (cBuf, &(nd->eg[d]));
        } else {
            // dbg ("leave failed\n");
            return i;
        }
        i++;
    }
    return i;
}

int fs0(struct node *nd)
{
     dbg_arg_n (1, "node %d fs %d\n", nd->num, nd->sig);
    for (int i=0; i<2; i++) { // phase 0: edge 0, 1
        if (nd->eg[i].pr.cp == 0) // no edge
            continue;
        strt (nd, i);
    }
    return 0;
}

int fs1(struct node *nd)
{
     dbg_arg_n (1, "node %d fs %d\n", nd->num, nd->sig);
    for (int i=2; i<4; i++) { // phase 1: edge 2, 3
        if (nd->eg[i].pr.cp == 0) // no edge
            continue;
        strt (nd, i);
    }
    return 0;
}

static void show_path (struct path *path)
{
    while (path) {
        dbg_arg_n (2, "%d ", path->n);
        path = path->next;
    }
    dbg ("\n");
}

static int rand_next(int n, int num)
{
    return (n + (rand() % (num -1)) + 1) % (num);
}

static int mrst_next(int n, int num) // more streight
{
    int d;
    d = rand()%num;
    if (d == n)
        d = get_oppo (d);
    return d;
}

struct car *rand_gene(struct simu *sm, struct node *nd, int n)
{   
    int d; // direction
    struct car *c;
    struct car **cTail;
    struct path **pTail;
    int idir;           // in direction
    int (*f)(int, int);

    f = mrst_next; // set next function

    dbg_arg_n (2, "node %d:\n", nd->num);
    if (nd->sig != n/2) // exit direction not green light
        return NULL;

    srand (time (0));

    c = NULL;
    cTail = &(c);
    idir = n;

    dbg_arg_n (1, "lunch %d cars\n", nd->eg[n].pr.p);
    for (int i=0; i<nd->eg[n].pr.p; i++) {
        struct node *nptr;
        int c;
        c = 0;

        nptr = nd;
        *cTail = malloc (sizeof(struct car));

        (*cTail)->onTm = 0;
        (*cTail)->next = NULL;
        (*cTail)->path = NULL;
        (*cTail)->id = 'A' + c_id;
        c_id = (c_id+1)%26;

        pTail = &((*cTail)->path);
        n = idir;
        dbg_arg_n (1, "in dir %d\n", n);
        do {
            dbg_arg_n (2, "(%d)", n);
            do {
                d = f (n, nptr->egNum);
            } while (nptr->eg[d].pr.cp < 1);
             dbg_arg_n (1, "dir %d, node %d\n", d, nptr->adj[d]);
            (*pTail) = malloc (sizeof(struct path));
            (*pTail)->n = d;
            (*pTail)->next = NULL;
            pTail = &((*pTail)->next);
            nptr = nptr->eg[d].from;
            n = get_oppo (d);
            c++;
        } while (nptr->type != N_EXIT); // find path until eixt
        //dbg ("");
        //show_path ((*cTail)->path);
#if DEBUG == 4
        g_total++;
        if (c>=3)
            long_p++;
        printf ("long rate: %lf\n", (double)long_p/g_total);
#endif
        cTail = &((*cTail)->next);
        nptr = NULL;
    }
    cTail = NULL;
    pTail = NULL;
    return c;
}
