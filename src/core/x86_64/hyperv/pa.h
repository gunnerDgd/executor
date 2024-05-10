#ifndef VAPOR_CORE_HYPERV_PA_H
#define VAPOR_CORE_HYPERV_PA_H

#include <core.h>

extern obj_trait *vp_pa_t;
typedef struct    vp_pa  {
    obj             head;
    struct vp_root *root;
    struct vp_cpu  *cpu;
    obj            *sub;
    u64_t           len;
    reg_t           pa;
}   vp_pa;

bool_t          vp_pa_new  (vp_pa*, u32_t, va_list);
bool_t          vp_pa_clone(vp_pa*, vp_pa*)        ;
void            vp_pa_del  (vp_pa*)                ;

struct vp_root *vp_pa_root (vp_pa*);
reg_t           vp_pa_begin(vp_pa*);
reg_t           vp_pa_end  (vp_pa*);
u64_t           vp_pa_len  (vp_pa*);
obj*            vp_pa_sub  (vp_pa*);

#endif
