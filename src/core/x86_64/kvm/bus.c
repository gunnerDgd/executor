#include "bus.h"
#include "pa.h"

obj_trait vp_bus_trait = make_trait (
    vp_bus_new    ,
    vp_bus_clone  ,
    null_t        ,
    vp_bus_del    ,
    sizeof(vp_bus),
    null_t
);

obj_trait *vp_bus_t = &vp_bus_trait;

bool_t
    vp_bus_new
        (vp_bus* self, u32_t count, va_list arg)                                 {
            vp_bus_ops *ops   = null_t; if (count > 0) ops = va_arg(arg, any_t);
            vp_pa      *pa    = null_t; if (count > 1) pa  = va_arg(arg, any_t);
            obj        *bus   = null_t; if (count > 2) bus = va_arg(arg, any_t);

            if (trait_of(pa)  != vp_pa_t) return false_t;
            if (trait_of(bus) == null_t)  return false_t;
            if (!ops)                     return false_t;

            self->bus = ref(bus);
            self->ops = ops;
            self->pa  = pa ;
            return true_t;
}

bool_t
    vp_bus_clone
        (vp_bus* self, vp_bus* clone) {
            return false_t;
}

void
    vp_bus_del
        (vp_bus* self)     {
            del (self->bus);
}