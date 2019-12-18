#ifndef _LIBSIM_H_
#define _LIBSIM_H_

struct simu;
struct node;
struct edge;
struct egpr;
struct car;

struct simu
{
    int ndNum;
    struct node *nd;
    int (*cr_gnr)(struct edge *);
};

struct node
{
    int egNum;
    struct edge *eg;
    int *adj;
    int ip;
    int fs;         // phase
    int sig;        // signal status
    int (**update)(struct node *); // update function
};

struct egpr      // edge parameter
{
    int cp;      // max capacity
    int p;       // forword cars / second
    int tw;      // time on edge 
};

struct edge
{
    struct node *from;
    struct egpr pr;
    int cont;           // current content
    struct car *cr;     // cats on this edge
};


struct car
{
    int onTm;
    int *path;
    struct car *next;
};

struct simu *init_simu(int (*)(struct edge *));
int free_simu(struct simu *);
int update(struct simu *);
int add_sig(struct simu *,int ,int , int (*fptr)(struct node *));
int add_all_sig(struct simu *,int , int (*fptr)(struct node *));

#endif
