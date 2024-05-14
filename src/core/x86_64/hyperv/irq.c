#include "irq.h"

#include "root.h"
#include "pic.h"

#include <WinHvPlatformDefs.h>
#include <WinHvPlatform.h>

ord_t
    vp_irq_do_ord
        (vp_irq* self, vp_irq* ord)                       {
            if (trait_of(self) != vp_irq_t) return ord_err;
            if (trait_of(ord)  != vp_irq_t) return ord_err;

            if (self->irq < ord->irq) return ord_lt;
            if (self->irq > ord->irq) return ord_gt;
            return ord_eq;
}

ord_t
    vp_irq_do_ord_arg
        (vp_irq* self, any_t ord)                         {
            if (trait_of(self) != vp_irq_t) return ord_err;
            if (self->irq < (u64_t) ord)    return ord_lt;
            if (self->irq > (u64_t) ord)    return ord_gt;
            return ord_eq;
}

ops_cmp vp_irq_do_cmp = make_ops_cmp (
    vp_irq_do_ord_arg,
    vp_irq_do_ord
);

obj_ops vp_irq_do      = { 
    .cmp = &vp_irq_do_cmp 
};

obj_trait vp_irq_trait = make_trait (
    vp_irq_new    ,
    vp_irq_clone  ,
    null_t        ,
    vp_irq_del    ,
    sizeof(vp_pic),
    &vp_irq_do
);

obj_trait *vp_irq_t = &vp_irq_trait;

bool_t 
    vp_irq_new
        (vp_irq* self, u32_t count, va_list arg)                         {
            vp_pic *pic = null_t; if (count > 0) pic = va_arg(arg, any_t);
            u64_t   irq = 0ull  ; if (count > 1) irq = va_arg(arg, u64_t);
            if (trait_of(pic) != vp_pic_t) return false_t;
            self->root = pic->root;
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

bool_t 
    vp_irq_clone
        (vp_irq* self, vp_irq* clone) {
            return false_t;
}

void   
    vp_irq_del
        (vp_irq* self) {
}

bool_t
    vp_irq_raise
        (vp_irq* self)                                    {
            if (trait_of(self) != vp_irq_t) return false_t;
            WHV_INTERRUPT_CONTROL arg                   = {
                .Vector          = self->arg.irq ,
                .TriggerMode     = self->arg.trig,
                .Type            = self->arg.type,
                .Destination     = self->arg.dest,
                .DestinationMode = self->arg.mode
            };

            if (WHvRequestInterrupt(self->root->root, &arg, sizeof(arg))) return false_t;
            return true_t;
}

void vp_irq_type(vp_irq* self, u64_t arg) { if (trait_of(self) == vp_irq_t) self->arg.type = arg; }
void vp_irq_mode(vp_irq* self, u64_t arg) { if (trait_of(self) == vp_irq_t) self->arg.mode = arg; }
void vp_irq_mask(vp_irq* self, u64_t arg) { if (trait_of(self) == vp_irq_t) self->arg.mask = arg; }
void vp_irq_dest(vp_irq* self, u64_t arg) { if (trait_of(self) == vp_irq_t) self->arg.dest = arg; }
void vp_irq_trig(vp_irq* self, u64_t arg) { if (trait_of(self) == vp_irq_t) self->arg.trig = arg; }