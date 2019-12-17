#ifndef _LIBSIM_H_
#define _LIBSIM_H_

struct simu;
struct node;
struct edge;
struct egpr;
struct car;`

struct simu
{
    int ndNum;
    struct *node nd;
};

struct node
{
    int egNum;
    struct *edge eg;
    int *adj;
    int ip;
    int fs;         // phase
    int sig;        // signal status
    int (**update)(struct node)// update function
};

struct edge
{
    struct node *from;
    struct egpr pr;
    int cont;           // current content
    struct car *cr;     // cats on this edge
};

struct egpr      // edge parameter
{
    int cp;      // max capacity
    int p;       // forword cars / second
    int tw;      // time on edge 
};

struct car
{
    int onTm;
    int *path;
    struct car *next;
};

struct simu *init_simu();
int free_simu(struct simu *);
int update(struct simu *);
int add_sig(int n, int (*fptr)(struct node *));

#endif
