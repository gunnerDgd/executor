#ifndef VAPOR_CORE_VA_H
#define VAPOR_CORE_VA_H

#include <core.h>

struct vp_cpu;
struct vp_pa;

extern obj_trait *vp_va_t;
typedef struct    vp_va { u8_t va[128]; } vp_va;

reg_t          vp_va_begin(vp_va*);
reg_t          vp_va_end  (vp_va*);
u64_t          vp_va_len  (vp_va*);
struct vp_cpu* vp_va_cpu  (vp_va*);
struct vp_pa*  vp_va_pa   (vp_va*);

#endif
