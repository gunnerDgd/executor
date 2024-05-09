#ifndef VAPOR_CORE_KVM_ROOT_H
#define VAPOR_CORE_KVM_ROOT_H

#include <core.h>
#include <collections.h>

struct vp_cpu;
struct vp_pa ;

extern obj_trait *vp_root_t;
typedef struct    vp_root  {
    obj   head;
    u64_t root;
    u64_t map;
    map   cpu,
          pa ;
}   vp_root;

bool_t         vp_root_new    (vp_root*, u32_t, va_list);
bool_t         vp_root_clone  (vp_root*, vp_root*)      ;
void           vp_root_del    (vp_root*)                ;

struct vp_cpu* vp_root_cpu    (vp_root*, u64_t)       ;
struct vp_pa*  vp_root_pa     (vp_root*, reg_t, u64_t);

#endif
