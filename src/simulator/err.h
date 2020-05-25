#ifndef _ERR_H_
#define _ERR_H_


#define dis_err 1

struct errm
{
    char *msg;
    struct errm *next;
};

void set_errm(char *s);
void pri_errm(char *s);
void clr_errm(void);


#if dis_err

#define serrm(n) set_errm(n)
#define perrm(n) pri_errm(n)
#define cerrm() clr_errm()

#else

#define serrm(n)
#define perrm(n)
#define cerrm()

#endif

#endif
