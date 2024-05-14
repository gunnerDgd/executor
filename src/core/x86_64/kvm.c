#include "kvm.h"

#include <linux/kvm_para.h>
#include <linux/kvm.h>

#include <unistd.h>
#include <fcntl.h>

obj_trait vp_core_trait = make_trait (
    vp_core_new    ,
    vp_core_clone  ,
    vp_core_ref    ,
    vp_core_del    ,
    sizeof(vp_core),
    null_t
);

obj_trait *vp_core_t = &vp_core_trait;

struct vp_core vp_core;

bool_t
    vp_core_new
        (struct vp_core* self, u32_t count, va_list arg) {
            self->kvm = open("/dev/kvm", O_RDWR);

            if (!make_at (&self->root, list) from (0)) goto new_err;
            if (self->kvm < 0)                         goto new_err;
            return true_t;
    new_err:
            del  (&self->root);
            close(self->kvm);
            self->kvm = -1;
            self->cpu =  0;
            self->mem =  0;
            return false_t;
}

bool_t vp_core_clone(struct vp_core* self, struct vp_core* clone) { return false_t; }
bool_t vp_core_ref  (struct vp_core* self)                        { return false_t; }

void
    vp_core_del
        (struct vp_core* self) {
            del  (&self->root);
            close(self->kvm)  ;
}