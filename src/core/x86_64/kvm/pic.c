#include "pic.h"

#include "root.h"
#include "irq.h"

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
            if (root->thd != this_thd())     return false_t;struct kvm_pit_config pit = { .flags = 0 };

            self->pic.chip_id = KVM_IRQCHIP_IOAPIC;
            self->root        = root;
            self->num         = 0   ;

            if (ioctl(root->root, KVM_GET_IRQCHIP, &self->pic)) return false_t;
            if (!make_at(&self->irq, list) from (0))            return false_t;
            return true_t;
}

bool_t vp_pic_clone(vp_pic* self, vp_pic* clone) { return false_t; }
void   vp_pic_del  (vp_pic* self)                {                 }

struct vp_irq*
    vp_pic_irq
        (vp_pic* self, u64_t irq)                        {
            if (trait_of(self) != vp_pic_t) return null_t;

            node *ret = map_find (&self->irq, (any_t) irq);
            if (trait_of(ret) != node_t) return null_t;
            return value_as(ret, vp_irq*);
}