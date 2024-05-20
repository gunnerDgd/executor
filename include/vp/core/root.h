#ifndef VAPOR_CORE_ROOT_H
#define VAPOR_CORE_ROOT_H

#include <core.h>

extern obj_trait *vp_root_t;
typedef struct    vp_root { u8_t root[1 KB]; } vp_root;

struct vp_cpu* vp_root_cpu  (vp_root*, u64_t)       ;
struct vp_map* vp_root_map  (vp_root*, reg_t, u64_t);
struct vp_pa*  vp_root_pa   (vp_root*, reg_t, u64_t);
void*          vp_root_ua   (vp_root*, reg_t, u64_t);

#endif
