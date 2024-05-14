#ifndef VAPOR_CORE_MAP_H
#define VAPOR_CORE_MAP_H

#include <core.h>

struct vp_pa;

extern obj_trait *vp_map_t;
typedef struct    vp_map { u8_t map[128]; } vp_map;

u64_t         vp_map_len  (vp_map*);
struct vp_pa* vp_map_pa   (vp_map*);
void*         vp_map_ua   (vp_map*);

#endif
