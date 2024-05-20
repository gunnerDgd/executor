#include "root.h"

#include "../kvm.h"
#include "cpu.h"
#include "pa.h"

#include <linux/kvm_para.h>
#include <linux/kvm.h>
#include <sys/ioctl.h>

#include <thread.h>

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
            struct kvm_pit_config pit = { .flags = 0 };
            u64_t                 map = 0xfeffc000;
            self->thd = this_thd();
            self->map = 0;

            if ((self->root = ioctl(vp_core.kvm, KVM_CREATE_VM, 0)) < 0)   goto new_err;
            if (ioctl(self->root, KVM_SET_TSS_ADDR         ,  map + 1 KB)) goto new_err;
            if (ioctl(self->root, KVM_SET_IDENTITY_MAP_ADDR, &map))        goto new_err;
            if (ioctl(self->root, KVM_CREATE_IRQCHIP       , 0))           goto new_err;
            if (ioctl(self->root, KVM_CREATE_PIT2          , &pit))        goto new_err;

            if (!make (vp_pa) from (3, self, map, 4 KB)) goto new_err;
            if (!list_move_back(&vp_core.root, self))    goto new_err;
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

struct vp_map*
    vp_root_map
        (vp_root* self, reg_t map, u64_t len)       {
            vp_pa  *pa  = vp_root_pa(self, map, len);
            vp_map *ret = vp_pa_sub (pa);

            if (trait_of(ret) != vp_map_t) return null_t;
            return ret;
}

struct vp_pa*
    vp_root_pa
        (vp_root* self, reg_t pa, u64_t len)              {
            if (trait_of(self) != vp_root_t) return null_t;

            vp_pa* ret = value_as (map_find(&self->pa, (any_t) pa), vp_pa*);
            if (trait_of (ret) != vp_pa_t) return null_t;
            if (vp_pa_len(ret) < len)      return null_t;
            return ret;
}

void*
    vp_root_ua
        (vp_root* self, reg_t ua, u64_t len)         {
            vp_map *map = vp_root_map (self, ua, len);
            vp_pa  *pa  = vp_map_pa   (map)          ;

            if (trait_of(map) != vp_map_t) return null_t;
            if (trait_of(pa)  != vp_pa_t)  return null_t;
            u8_t *ret = vp_map_ua(map);

            ret  -= vp_pa_begin(pa);
            ret  += ua;
            return ret;
}