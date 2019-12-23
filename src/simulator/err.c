#include <stdio.h>
#include <stdlib.h>

#include "err.h"

static struct errm *err_msg_lst = NULL;

void set_errm(char *s)
{
    struct errm **ptr;
    ptr = &err_msg_lst;
    while (*ptr != NULL)
    {
        ptr = &((*ptr)->next);
    }
    *ptr = malloc (sizeof(struct errm));
    (*ptr)->msg = s;
    (*ptr)->next = NULL;
}

void pri_errm(char *s)
{

    struct errm **ptr;

    printf ("%s:\n", s);
    ptr = &err_msg_lst;
    while (*ptr != NULL)
    {
        printf ("%s\n", (*ptr)->msg);
        ptr = &((*ptr)->next);
    }
}

void clr_errm(void)
{
    struct errm *buf;
    while (err_msg_lst != NULL)
    {
        buf = err_msg_lst;
        err_msg_lst = err_msg_lst->next;
        free (buf);
    }
}
