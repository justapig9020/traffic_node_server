#ifndef _METHOD_H_
#define _METHOD_H_

#include "libsim.h"

// Node Update Function
enum {
    NUF_FS0,
    NUF_FS1,
    NUF_FRSO
};

int fs0(struct node *);
int fs1(struct node *);
int frs_out(struct node *);
struct car *rand_gene(struct simu *, struct node *, int);

#endif
