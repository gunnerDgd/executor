#include "irq.h"

#include "root.h"
#include "pic.h"

#include <linux/kvm_para.h>
#include <linux/kvm.h>

#include <sys/ioctl.h>
#include <unistd.h>

obj_trait vp_irq_trait = make_trait (
    vp_irq_new  ,
    vp_irq_clone,
    null_t      ,
    vp_irq_del  ,
    sizeof(vp_irq),
    null_t
);

obj_trait *vp_irq_t = &vp_irq_trait;

bool_t
    vp_irq_new
        (vp_irq* self, u32_t count, va_list arg)                          {
            vp_pic *pic = null_t; if (count > 0) pic = va_arg (arg, any_t);
            u64_t   irq = 0ull  ; if (count > 1) irq = va_arg (arg, u64_t);
            if (trait_of(pic) != vp_pic_t)       return false_t;
            if (pic->num >= KVM_IOAPIC_NUM_PINS) return false_t;
            self->root = pic->root;
            self->num  = pic->num;
            self->pic  = pic;
            self->irq  = irq;

            if (!map_move(&pic->irq, self)) return false_t;
            self->arg.irq  = irq;
            self->arg.mode = 0;
            self->arg.mask = 0;
            self->arg.dest = 0;
            self->arg.stat = 0;
            self->arg.trig = 0;
            self->arg.pol  = 0;
            self->arg.irr  = 0;
            return true_t;
}

bool_t vp_irq_clone(vp_irq*, vp_irq*)       ;
void   vp_irq_del  (vp_irq*)                ;

bool_t
    vp_irq_raise
        (vp_irq* self)                                    {
            if (trait_of(self) != vp_irq_t) return false_t;

            struct kvm_irqchip      *irq = &self->pic->pic  ;
            struct kvm_ioapic_state *pic = &irq->chip.ioapic;
            u64_t                    num = self->num        ;

            pic->redirtbl[num].fields.delivery_mode = self->arg.type;
            pic->redirtbl[num].fields.dest_mode     = self->arg.mode;
            pic->redirtbl[num].fields.dest_id       = self->arg.dest;
            pic->redirtbl[num].fields.trig_mode     = self->arg.trig;
            pic->redirtbl[num].fields.vector        = self->arg.irq ;

            if (ioctl(self->root->root, KVM_SET_IRQCHIP, &self->pic->pic)) return false_t;
            if (ioctl(self->root->root, ))

}

void
    vp_irq_type
        (vp_irq* self, u64_t arg)                 {
            if (trait_of(self) != vp_irq_t) return;

            struct kvm_irqchip      *irq = &self->pic->pic  ;
            struct kvm_ioapic_state *pic = &irq->chip.ioapic;
            u64_t                    num = self->num        ;

}
void   vp_irq_mode (vp_irq*, u64_t);
void   vp_irq_mask (vp_irq*, u64_t);
void   vp_irq_dest (vp_irq*, u64_t);
void   vp_irq_trig (vp_irq*, u64_t);