#include "map.h"

#include "root.h"
#include "pa.h"

#include <linux/kvm_para.h>
#include <linux/kvm.h>
#include <sys/ioctl.h>

obj_trait vp_map_trait = make_trait (
    vp_map_new    ,
    vp_map_clone  ,
    null_t        ,
    vp_map_del    ,
    sizeof(vp_map),
    null_t
);

obj_trait *vp_map_t = &vp_map_trait;

bool_t
    vp_map_new
        (vp_map* self, u32_t count, va_list arg)                        {
            vp_pa *pa  = null_t; if (count > 0) pa  = va_arg(arg, any_t);
            void  *ua  = null_t; if (count > 1) ua  = va_arg(arg, any_t);
            u64_t  len = 0ull  ; if (count > 2) len = va_arg(arg, u64_t);

            if (trait_of(pa) != vp_pa_t) return false_t;
            if (len != pa->len)          return false_t;
            if (!ua)                     return false_t;

            self->uid = pa->root->map;
            self->len = len;
            self->pa  = pa;
            self->ua  = ua;

            struct kvm_userspace_memory_region map = {
                .guest_phys_addr = pa->pa      ,
                .userspace_addr  = (u64_t) ua  ,
                .memory_size     = len         ,
                .slot            = self->uid
            };

            if (ioctl (pa->root->root, KVM_SET_USER_MEMORY_REGION, &map) < 0) return false_t;
            pa->sub = (obj*) self;
            pa->root->map++;
            return true_t;
}

bool_t
    vp_map_clone
        (vp_map* self, vp_map* clone) {
            return false_t;
}

void
    vp_map_del
        (vp_map* self)                               {
            struct kvm_userspace_memory_region map = {
                .guest_phys_addr = self->pa->pa    ,
                .userspace_addr  = (u64_t) self->ua,
                .memory_size     = 0               ,
                .slot            = self->uid
            };

            vp_root *root = null_t;
            vp_pa   *pa   = null_t;
            pa   = self->pa;
            root = pa->root;

            if (trait_of(root) != vp_root_t) return;
            ioctl                                  (
                root->root                ,
                KVM_SET_USER_MEMORY_REGION,
                &map
            );
}

u64_t
    vp_map_len
        (vp_map* self)                              {
            if (trait_of(self) != vp_map_t) return 0;
            return self->len;
}

struct vp_pa*
    vp_map_pa
        (vp_map* self)                                   {
            if (trait_of(self) != vp_map_t) return null_t;
            return self->pa;
}

void*
    vp_map_ua
        (vp_map* self)                                   {
            if (trait_of(self) != vp_map_t) return null_t;
            return self->ua;
}