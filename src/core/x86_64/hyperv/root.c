#include "root.h"

#include "../hyperv.h"
#include "cpu.h"
#include "pa.h"


obj_trait vp_root_trait = make_trait (
    vp_root_new    ,
    vp_root_clone  ,
    null_t         ,
    vp_root_del    ,
    sizeof(vp_root),
    null_t
);

obj_trait *vp_root_t = &vp_root_trait;

bool_t
    vp_root_new
        (vp_root* self, u32_t count, va_list arg)                  {
            if (!make_at (&self->cpu, map) from (0)) return false_t;
            if (!make_at (&self->pa , map) from (0)) return false_t;
            if (WHvCreatePartition(&self->root))     return false_t;
            return true_t;
    new_err:
            del (&self->cpu);
            del (&self->pa) ;
            return false_t;
}

bool_t
    vp_root_clone
        (vp_root* self, vp_root* clone) {
            return false_t;
}

void
    vp_root_del
        (vp_root* self)     {
            del (&self->cpu);
            del (&self->pa) ;
}

struct vp_cpu* vp_root_cpu    (vp_root*, u64_t)       ;
struct vp_pa*  vp_root_pa     (vp_root*, reg_t, u64_t);