#ifndef VAPOR_FIRMWARE_H
#define VAPOR_FIRMWARE_H

#include "core.h"

#include "firmware/linux.h"

typedef struct vp_firm_ops              {
    bool_t  (*prep_root)(obj*, vp_root*);
    bool_t  (*prep_cpu) (obj*, vp_cpu *);
}   vp_firm_ops;

#define vp_make_firm_ops(par_root, par_cpu)          {\
    .prep_root = (bool_t (*)(obj*, vp_root*))par_root,\
    .prep_cpu  = (bool_t (*)(obj*, vp_cpu *))par_cpu  \
}\

extern obj_trait *vp_firm_t;
typedef struct    vp_firm  {
    obj          head;
    obj         *firm;
    vp_firm_ops *ops;
}   vp_firm;

bool_t  vp_firm_new      (vp_firm*, u32_t, va_list);
bool_t  vp_firm_clone    (vp_firm*, vp_firm*)      ;
void    vp_firm_del      (vp_firm*)                ;

bool_t  vp_firm_prep_root(vp_firm*, vp_root*);
bool_t  vp_firm_prep_cpu (vp_firm*, vp_cpu *);

#endif
