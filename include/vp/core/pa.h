#ifndef VAPOR_CORE_PA_H
#define VAPOR_CORE_PA_H

#include <core.h>

struct vp_root;

extern obj_trait *vp_pa_t;
typedef struct    vp_pa { u8_t pa[128]; } vp_pa;

struct vp_root *vp_pa_root (vp_pa*);
reg_t           vp_pa_begin(vp_pa*);
reg_t           vp_pa_end  (vp_pa*);
u64_t           vp_pa_len  (vp_pa*);
obj*            vp_pa_sub  (vp_pa*);

#endif
