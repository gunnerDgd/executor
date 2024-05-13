#include "root.h"

#include "../kvm.h"
#include "cpu.h"
#include "pa.h"

#include <linux/kvm_para.h>
#include <linux/kvm.h>
#include <sys/ioctl.h>

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
            self->map = 0;

            if ((self->root = ioctl(vp_core.kvm, KVM_CREATE_VM, 0)) < 0)         goto new_err;
            if (ioctl(self->root, KVM_SET_TSS_ADDR         , 0xfeffc000 + 1 KB)) goto new_err;
            if (ioctl(self->root, KVM_SET_IDENTITY_MAP_ADDR, 0xfeffc000))        goto new_err;
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

struct vp_cpu*
    vp_root_cpu
        (vp_root* self, u64_t cpu)                        {
            if (trait_of(self) != vp_root_t) return null_t;
            return value_as                      (
                map_find(&self->cpu, (any_t) cpu),
                vp_cpu*
            );
}

struct vp_pa*
    vp_root_pa
        (vp_root* self, reg_t pa, u64_t len)              {
            if (trait_of(self) != vp_root_t) return null_t;

            vp_pa* ret = value_as (map_find(&self->pa, (any_t) pa), vp_pa*);
            if (trait_of(pa) != vp_pa_t) return null_t;
            if (vp_pa_len(ret) > len)    return null_t;
            return ret;
}