#ifndef VAPOR_CORE_KVM_MAP_H
#define VAPOR_CORE_KVM_MAP_H

#include <core.h>

struct vp_root;
struct vp_pa;

extern obj_trait *vp_map_t;
typedef struct    vp_map  {
    obj           head;
    u64_t         len;
    u64_t         uid;
    struct vp_pa *pa;
    void         *ua;
}   vp_map;

bool_t        vp_map_new  (vp_map*, u32_t, va_list);
bool_t        vp_map_clone(vp_map*, vp_map*)       ;
void          vp_map_del  (vp_map*)                ;

u64_t         vp_map_len  (vp_map*);
struct vp_pa* vp_map_pa   (vp_map*);
void*         vp_map_ua   (vp_map*);

#endif