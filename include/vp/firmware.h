#ifndef VAPOR_FIRMWARE_H
#define VAPOR_FIRMWARE_H

#include "firmware/linux.h"

struct vp_root;
struct vp_cpu;

typedef struct vp_firm_ops                     {
    bool_t  (*prep_root)(obj*, struct vp_root*);
    bool_t  (*prep_cpu) (obj*, struct vp_cpu *);
}   vp_firm_ops;

#define vp_make_firm_ops(par_root, par_cpu)          {\
    .prep_root = (bool_t (*)(obj*, vp_root*))par_root,\
    .prep_cpu  = (bool_t (*)(obj*, vp_cpu *))par_cpu  \
}\
extern obj_trait *vp_firm_t;
typedef struct    vp_firm { u8_t firm[64]; } vp_firm;

bool_t  vp_firm_prep_root(vp_firm*, struct vp_root*);
bool_t  vp_firm_prep_cpu (vp_firm*, struct vp_cpu *);

#endif
