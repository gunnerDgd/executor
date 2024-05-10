#ifndef VAPOR_CORE_HYPERV_VA_H
#define VAPOR_CORE_HYPERV_VA_H

#include <core.h>

struct vp_cpu;
struct vp_pa;

extern obj_trait *vp_va_t;
typedef struct    vp_va  {
    obj            head;
    struct vp_cpu *cpu;
    u64_t          len;
    struct vp_pa  *pa;
    reg_t          va;
}   vp_va;

bool_t         vp_va_new  (vp_va*, u32_t, va_list);
bool_t         vp_va_clone(vp_va*, vp_va*)        ;
void           vp_va_del  (vp_va*)                ;

reg_t          vp_va_begin(vp_va*);
reg_t          vp_va_end  (vp_va*);
u64_t          vp_va_len  (vp_va*);
struct vp_cpu* vp_va_cpu  (vp_va*);
struct vp_pa*  vp_va_pa   (vp_va*);

#endif
