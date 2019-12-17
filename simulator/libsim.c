#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "libsim.h"
#include "err.h"
#include "debug.h"
#include "conf.h"


// find node number via ip
static int find_nd_num(struct simu *s, int ip)
{
    for (int i=0; i<sm->ndNum; i++) {
        if (ip == sm->nd[i].ip)
            return i;
    }
    return -1;
}

static int init_edge(FILE *fd, FILE *fd1, struct node *nptr)
{
    struct edge *eptr;
    char *s;

    eptr = &(nptr->eg[o]);
    s = malloc (16);
    
    for (int i=0; i<nptr->egNum; i++) {
        bzero (s, 16);
        fscanf (fd, "%s", s); // get adj_ip

        if (!strcmp (s, "NULL")) { // not NULL set to to the node
            int buf = inet_addr (s);
            int nbuf = find_nd_num (buf);
            if (nbuf == -1) {     // node not fount
                dbg_arg ("adj_ip not found @ node %d edge %d\n", i, o);
                eptr->from = NULL;
                nptr[o]->adj = -1;
            } else {
                eptr->from = &(sm->nd[nbuf]);
                nptr[o]->adj = nbuf;
            }
        } else {                  // NULL set NULL to to
            eptr->to = NULL; 
            nptr[o]->adj = -1;
        }

        // set edge parameter
        fscanf (fd1, "%d", &(eptr->pr.cp));
        fscanf (fd1, "%d", &(eptr->pr.p));
        fscanf (fd1, "%d", &(eptr->pr.tw));

        // init edge status
        eptr->cont = 0;
        eptr->cr = NULL;
    }
    return 0;
}

static int eg_update (struct simu *sm, struct node *nptr)
{
    for (int i=0; i<nptr->egNum; i++) {
        struct edge *eptr;

        eptr = &(nptr->eg[i]);

        if (eptr->pr > 0) { // 

        } else if (eptr->pr.cp == -1) { // exit 
                // car generate
        }
    }

}

struct simu *init_simu()
{
    struct simu *sm;
    FILE *fd;
    FILE *fd1;
    char *fn;
    char *s;

    sm = malloc (sizeof(struct simu));
    fn = malloc (20);
    s = malloc (16);

    sprintf (fn, "%s%s",CONF_DIR, IP_TBL); // ip_table
    fd = fopen (fn, "r");

    fscanf (fd, "%d", &(sm->ndNum));         // get node amount
    sm->nd = malloc (sizeof(struct node) * sm->num);

    // set each node's ip
    for (int i=0; i<sm->num; i++) {
        bzero (s, 16);
        fscanf (fd, "%s", s);
        sm->nd[i].ip = inet_addr (s);
    }

    fclose (fd);
    fd = NULL;

    // read each node's adj_table and sim_table
    for (int i=0; i<sm->num; i++) {
        int n;
        struct node *nptr;

        nptr = &(sm->nd[i]);

        // adj_table
        sprintf (fn+CONF_DIR_SIZE, "%s%d", ADJ_TBL, i);
        fd = fopen (fn, "r");
        
        // sim_table
        sprintf (fn+CONF_DIR_SIZE, "%s%d", SIM_TBL, i);
        fd1 = fopen (fn, "r");


        fscanf (fd, "%d", &n); // get adj amount
        nptr->egNum = n;
        nptr->eg = malloc (sizeof(struct edge) * n);
        nptr->adj = malloc (sizeof(int) * n);

        fscanf (fd1, "%d", &n); // get adj amount
        fscanf (fd1, "%d", &n); // get phase amount
        nptr->fs = n;
        nptr->update = malloc (sizeof(int(*)()) * n); // set update function number
    
        // init edge_para
        init_edge (fd, fd1, nptr);
    }
    return sm;
}

int free_simu(struct simu* sm)
{
    for (int i=0; i<sm->ndNum; i++) {
        struct node *nptr = &(sm->nd[i]);

        free (nptr->eg);
        nptr->eg = NULL;
        free (nptr_>adj);
        nptr->adj = NULL;
        free (nptr->update);
        nptr->update = NULL;
        
    }
    free (sm->nd);
    sm->nd = NULL;
    free(sm);
    sm = NULL;
    return 0;
}

int update(struct simu *sm)
{
    for (int i=0; i<sm->ndNum; i++) {
        struct node *nptr;

        eg_update (sm, nptr);
        nptr = &(sm->nd[i]);
        nptr->update[nptr->sig] (nptr);
    }
    return 0;
}

int add_sig(struct simu *sm, int n, int s, int (*fptr)(struct node *))
{
    sm->nd[n].update[s] = fptr;
    return 0;
}
