#ifndef _LIBSIM_H_
#define _LIBSIM_H_

enum{
    N_EXIT,
    N_CROS
};

struct simu;
struct node;
struct edge;
struct egpr;
struct path;
struct car;
struct ndbuf;

struct simu
{
    int ndNum;
    struct node *nd;
    struct car *(*cr_gnr)(struct simu *, struct node *, int);
};

struct node
{
    int num;
    int egNum;
    int type;
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

struct path
{
    int n;
    struct path *next;
};

struct car
{
    int onTm;
    struct path *path;
    struct car *next;
};

struct ndbuf
{
    struct node *nd;
    struct ndbuf *next;
};

//struct simu sm;

int get_oppo(int);
struct simu *init_simu();
int set_cr_gnr(struct simu *, struct car *(*)(struct simu *, struct node *, int));
int free_simu(struct simu *);
int update(struct simu *);
int add_sig(struct simu *,int ,int , int (*fptr)(struct node *));
int add_all_sig(struct simu *,int , int (*fptr)(struct node *));

void show_eg_conf(struct edge *);
void show_nd_conf(struct node *);
void show_sm_conf(struct simu *);

void show_pt(struct path *);
void show_car(struct car *);
void show_eg(struct edge *);
void show_nd(struct node *);
void show_sm(struct simu *);


#endif
