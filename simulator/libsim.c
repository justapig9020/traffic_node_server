#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define DEBUG 0
#include "libsim.h"
#include "debug.h"
#include "conf.h"


int get_oppo(int n) {
    return ((n + 1)&1) + (n & ~1);
}

// find node number via ip
static int find_nd_num(struct simu *sm, int ip)
{
    for (int i=0; i<sm->ndNum; i++) {
        dbg_arg ("ip cmp: %d %d\n", ip, sm->nd[i].ip);
        if (ip == sm->nd[i].ip)
            return i;
    }
    return -1;
}

static int init_edge(struct simu *sm, FILE *fd, FILE *fd1, struct node *nptr)
{
    struct edge *eptr;
    char s[16]; // set ip string 

    nptr->type = N_CROS; // node type: N_CROS(cross) or N_EXIT(exit)
    for (int i=0; i<nptr->egNum; i++) {
        eptr = &(nptr->eg[i]);

        bzero (s, 16);
        fscanf (fd, "%s", s); // get adj_ip
        dbg_arg ("ip: %s\n", s);

        if (strcmp (s, "NULL")) { // not NULL set to to the node
            int buf = inet_addr (s);
            int nbuf = find_nd_num (sm, buf);
            if (nbuf == -1) {     // node not fount
                dbg_arg ("adj_ip not found, node %d, edge %d\n", nptr->num, i);
                eptr->from = NULL;
                nptr->adj[i]= -1;
            } else {
                dbg_arg ("adj_ip, node %d, edge %d\n", nptr->num, i);
                eptr->from = &(sm->nd[nbuf]);
                nptr->adj[i] = nbuf;
            }
        } else {                  // NULL set NULL to to
            eptr->from = NULL; 
            nptr->adj[i] = -1;
        }

        // set edge parameter
        fscanf (fd1, "%d", &(eptr->pr.cp));
        if (eptr->pr.cp == -1) { // this node has exit
            nptr->type = N_EXIT;
        }
        fscanf (fd1, "%d", &(eptr->pr.p));
        fscanf (fd1, "%d", &(eptr->pr.tw));

        // init edge status
        eptr->cont = 0;
        eptr->cr = NULL;
    }
    return 0;
}

static int eg_update(struct simu *sm, struct node *nptr)
{
    for (int i=0; i<nptr->egNum; i++) {
        struct edge *eptr;

        eptr = &(nptr->eg[i]);

        if (eptr->pr.cp > 0) { // edge exist
            struct car *cptr;

            cptr = eptr->cr;
            while (cptr) {      // every car forward one
                if (cptr->onTm > 0)
                    cptr->onTm--;
                cptr = cptr->next;
            }

        } else if (eptr->pr.cp == -1) { // exit 
            dbg_arg ("edge %d generating car\n", i);
            if ((nptr->eg[i].cr = sm->cr_gnr(sm, nptr, i))) { // generate car to opposite
                nptr->eg[i].cont = nptr->eg[i].pr.p;
            }
            dbg_arg ("%p\n", nptr->eg[i].cr);
        } else {
            // dbg ("edge not exist");
        }
    }
    return 0;
}

struct simu *init_simu()
{
    struct simu *sm;
    FILE *fd;
    FILE *fd1;
    char fn[20]; // file name
    char s[16];  // ip string 

    sm = malloc (sizeof(struct simu));

    sprintf (fn, "%s%s",CONF_DIR, IP_TBL); // ip_table
    dbg_arg ("%s\n", fn);
    fd = fopen (fn, "r");

    fscanf (fd, "%d", &(sm->ndNum));         // get node amount
    sm->nd = malloc (sizeof(struct node) * sm->ndNum);

    // set each node's ip
    for (int i=0; i<sm->ndNum; i++) {
        bzero (s, 16);
        fscanf (fd, "%s", s);
        sm->nd[i].ip = inet_addr (s); // set ip as int
        sm->nd[i].num = i;            // set node number
    }

    fclose (fd);
    fd = NULL;

    // read each node's adj_table and sim_table
    for (int i=0; i<sm->ndNum; i++) {
        int n;
        struct node *nptr;

        nptr = &(sm->nd[i]);

        // adj_table
        sprintf (fn+CONF_DIR_SIZE, "%s%d", ADJ_TBL, i);
        dbg_arg ("f1: %s\n", fn);
        fd = fopen (fn, "r");
        
        // sim_table
        sprintf (fn+CONF_DIR_SIZE, "%s%d", SIM_TBL, i);
        dbg_arg ("f2: %s\n", fn);
        fd1 = fopen (fn, "r");


        fscanf (fd, "%d", &n); // get adj amount
        nptr->egNum = n;
        nptr->eg = malloc (sizeof(struct edge) * n);
        nptr->adj = malloc (sizeof(int) * n);

        fscanf (fd1, "%d", &n); // get adj amount
        fscanf (fd1, "%d", &n); // get phase amount
        nptr->fs = n;
        nptr->update = malloc (sizeof(int(*)(struct node *)) * n); // set update function number
    
        // init edge_para
        init_edge (sm, fd, fd1, nptr);
    }
    return sm;
}

int set_cr_gnr(struct simu *sm, struct car *(*f)(struct simu *, struct node *, int))
{
    // set car genetation funciotn
    sm->cr_gnr = f;
    return 0;
}

int free_simu(struct simu* sm)
{
    for (int i=0; i<sm->ndNum; i++) {
        struct node *nptr = &(sm->nd[i]);

        free (nptr->eg);
        nptr->eg = NULL;
        free (nptr->adj);
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

        dbg_arg ("updating node %d\n", i);
        nptr = &(sm->nd[i]);
        eg_update (sm, nptr);
        //puts ("");
        //show_nd (nptr);
        nptr->update[nptr->sig] (nptr);
        //puts ("");
        //show_nd (nptr);
        //getchar ();
    }
    return 0;
}


int add_sig(struct simu *sm, int n, int s, int (*fptr)(struct node *))
{
    sm->nd[n].update[s] = fptr;
    return 0;
}

int add_all_sig(struct simu *sm, int s, int(*fptr)(struct node *))
{
    for (int i=0; i<sm->ndNum; i++) {
        add_sig (sm, i, s, fptr);
    }
    return 0;
}


void show_eg_conf(struct edge *eg)
{
    printf ("Capacity: %d\nP: %d\nTw: %d\n", eg->pr.cp, eg->pr.p, eg->pr.tw);
}

void show_nd_conf(struct node *nd)
{
    printf ("Edges: %d\nPhase: %d\nType: %s\n", nd->egNum, nd->fs, nd->type == N_CROS? "CROSS":"EXIT");
    for (int i=0; i<nd->egNum; i++) {

        printf ("  = Edge %d =\n", i);
        printf ("form %p\n", nd->eg[i].from);
        show_eg_conf (&(nd->eg[i]));
    }
}

void show_sm_conf(struct simu *sm)
{
    printf ("==== Show simulater config====\nNodes: %d\n", sm->ndNum);
    for (int i=0; i<sm->ndNum; i++) {
        printf ("\n === Node %d ===\n", i);
        show_nd_conf (&(sm->nd[i]));
    }
}

void show_path(struct path **pt)
{
    if (*pt) {
        printf ("%d ", (*pt)->n);
        pt = &((*pt)->next);
    } else {
        printf ("E ");
    }
}

int show_car(struct car *cr)
{
    int i;
    i=0;
    while (cr) {
        for (;i<cr->onTm; i++)
            printf ("* ");
        //show_path (&(cr->path));
        printf ("%c ", cr->id);
        cr = cr->next;
    }
    return i;
}

void show_eg(struct edge *eg)
{
    int i;
    if (eg->pr.cp == -1)
        printf ("# ");
    else
        printf ("  ");

    printf ("c: %d |  ", eg->cont);
    i = show_car (eg->cr);

    //for (; i<eg->pr.cp; i++)
    //    printf ("* ");
    //printf ("")
}

void show_nd(struct node *nd)
{
    printf ("\n\n === Node %d ===", nd->num);
    printf (" Signal: %d", nd->sig);
    for (int i=0; i<nd->egNum; i++) {
        if (nd->eg[i].pr.cp == 0)
            continue;
        printf ("\n = Edge %d = \n", i);
        show_eg (&(nd->eg[i]));
    }
}


void show_sm(struct simu *sm)
{
    printf ("==== Show simulater ====\n");
    for (int i=0; i<sm->ndNum; i++) {
        show_nd (&(sm->nd[i]));
    }
}
