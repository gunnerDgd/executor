#include "pic.h"
#include "root.h"

#include <linux/kvm_para.h>
#include <linux/kvm.h>

#include <sys/ioctl.h>
#include <unistd.h>

obj_trait vp_pic_trait = make_trait (
    vp_pic_new    ,
    vp_pic_clone  ,
    null_t        ,
    vp_pic_del    ,
    sizeof(vp_pic),
    null_t
);

obj_trait *vp_pic_t = &vp_pic_trait;

bool_t
    vp_pic_new
        (vp_pic *self, u32_t count, va_list arg)                            {
            vp_root *root = null_t; if (count > 0) root = va_arg(arg, any_t);
            if (trait_of(root) != vp_root_t) return false_t;
            self->root = root;

            if (ioctl(root->root, KVM_CREATE_IRQCHIP)) return false_t;
            if (ioctl(root->root, KVM_CREATE_PIT2))    return false_t;
            if (!make_at(&self->irq, list) from (0))   return false_t;
            return true_t;
}

bool_t vp_pic_clone(vp_pic* self, vp_pic* clone) { return false_t; }
void   vp_pic_del  (vp_pic* self)                {                 }