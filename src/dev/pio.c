#include "pio.h"

ord_t
    vp_pio_dev_do_ord_arg
        (vp_pio_dev *self, any_t arg)                         {
            if (trait_of(self) != vp_pio_dev_t) return ord_err;

            if (self->port < arg) return ord_lt;
            if (self->port > arg) return ord_gt;
            return ord_eq;
}

ord_t
    vp_pio_dev_do_ord
        (vp_pio_dev *self, vp_pio_dev *arg)                   {
            if (trait_of(self) != vp_pio_dev_t) return ord_err;
            if (trait_of(arg)  != vp_pio_dev_t) return ord_err;

            if (self->port < arg->port) return ord_lt;
            if (self->port > arg->port) return ord_gt;
            return ord_eq;
}

ops_cmp vp_pio_do_cmp = make_ops_cmp (
    vp_pio_dev_do_ord    ,
    vp_pio_dev_do_ord_arg
);

obj_ops vp_pio_do     = {
    .cmp = &vp_pio_do_cmp
};

obj_trait vp_pio_dev_trait = make_trait (
    vp_pio_dev_new    ,
    vp_pio_dev_clone  ,
    null_t            ,
    vp_pio_dev_del    ,
    sizeof(vp_pio_dev),
    &vp_pio_do
);

obj_trait *vp_pio_dev_t = &vp_pio_dev_trait;

bool_t
    vp_pio_dev_new
        (vp_pio_dev* self, u32_t count, va_list arg)                           {
            vp_pio     *pio  = null_t; if (count > 0) pio  = va_arg(arg, any_t);
            u64_t       port = 0ull  ; if (count > 1) port = va_arg(arg, u64_t);
            vp_pio_ops *ops  = null_t; if (count > 2) ops  = va_arg(arg, any_t);
            obj        *dev  = null_t; if (count > 3) dev  = va_arg(arg, any_t);

            if (trait_of(pio) != vp_pio_t) return false_t;
            if (trait_of(dev) == null_t)   return false_t;
            if (!ops)                      return false_t;

            self->port = port;
            self->pio  = pio;
            self->dev  = dev;
            self->ops  = ops;

            if (!map_move(&pio->pio, self)) return false_t;
            return true_t;
}

bool_t vp_pio_dev_clone(vp_pio_dev* self, vp_pio_dev* clone) { return false_t; }
void   vp_pio_dev_del  (vp_pio_dev* self)                    {                 }