#ifndef VAPOR_CORE_KVM_H
#define VAPOR_CORE_KVM_H

#include <core.h>
#include <collections.h>

extern obj_trait *vp_core_t;
struct            vp_core  {
    obj   head;
    list  root;
    u64_t kvm;
    u64_t cpu;
    u64_t mem;
}   vp_core;

bool_t vp_core_new  (struct vp_core*, u32_t, va_list) ;
bool_t vp_core_clone(struct vp_core*, struct vp_core*);
bool_t vp_core_ref  (struct vp_core*);
void   vp_core_del  (struct vp_core*);

#endif
