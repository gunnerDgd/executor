#include "bus.h"

#include "root.h"
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
        (vp_bus* self, u32_t count, va_list arg)                             {
            vp_bus_ops *ops = null_t; if (count > 0) ops = va_arg(arg, any_t);
            obj        *bus = null_t; if (count > 1) bus = va_arg(arg, any_t);
            vp_pa      *pa  = null_t; if (count > 2) pa  = va_arg(arg, any_t);
            if (trait_of(pa)  != vp_pa_t)    return false_t;
            if (trait_of(bus) == null_t)     return false_t;

            if (pa->root->thd != this_thd()) return false_t;
            if (!ops)                        return false_t;

            self->bus = bus;
            self->ops = ops;
            self->pa  = pa ;
            return true_t;
}

bool_t vp_bus_clone (vp_bus* self, vp_bus* clone) { return false_t; }
void   vp_bus_del   (vp_bus* self)                {                 }

bool_t
    vp_bus_in8
        (vp_bus* self, reg_t addr, u8_t* buf)             {
            if (trait_of(self) != vp_bus_t) return false_t;
            if (!buf)                       return false_t;

            if (vp_pa_begin(self->pa) <  addr) return false_t;
            if (vp_pa_end  (self->pa) >= addr) return false_t;
            return self->ops->in8                            (
                self->bus,
                addr     ,
                buf
            );
}

bool_t
    vp_bus_in16
        (vp_bus* self, reg_t addr, u16_t* buf)             {
            if (trait_of(self) != vp_bus_t) return false_t;
            if (!buf)                       return false_t;

            if (vp_pa_begin(self->pa) <  addr) return false_t;
            if (vp_pa_end  (self->pa) >= addr) return false_t;
            return self->ops->in16                           (
                self->bus,
                addr     ,
                buf
            );
}

bool_t
    vp_bus_in32
        (vp_bus* self, reg_t addr, u32_t* buf)            {
            if (trait_of(self) != vp_bus_t) return false_t;
            if (!buf)                       return false_t;

            if (vp_pa_begin(self->pa) <  addr) return false_t;
            if (vp_pa_end  (self->pa) >= addr) return false_t;
            return self->ops->in32                           (
                self->bus,
                addr     ,
                buf
            );
}

bool_t
    vp_bus_in64
        (vp_bus* self, reg_t addr, u64_t* buf)            {
            if (trait_of(self) != vp_bus_t) return false_t;
            if (!buf)                       return false_t;

            if (vp_pa_begin(self->pa) <  addr) return false_t;
            if (vp_pa_end  (self->pa) >= addr) return false_t;
            return self->ops->in64                           (
                self->bus,
                addr     ,
                buf
            );
}


bool_t
    vp_bus_out8
        (vp_bus* self, reg_t addr, u8_t* buf)             {
            if (trait_of(self) != vp_bus_t) return false_t;
            if (!buf)                       return false_t;

            if (vp_pa_begin(self->pa) <  addr) return false_t;
            if (vp_pa_end  (self->pa) >= addr) return false_t;
            return self->ops->out8                           (
                self->bus,
                addr     ,
                buf
            );
}

bool_t
    vp_bus_out16
        (vp_bus* self, reg_t addr, u16_t* buf)            {
            if (trait_of(self) != vp_bus_t) return false_t;
            if (!buf)                       return false_t;

            if (vp_pa_begin(self->pa) <  addr) return false_t;
            if (vp_pa_end  (self->pa) >= addr) return false_t;
            return self->ops->out16                          (
                self->bus,
                addr     ,
                buf
            );
}

bool_t
    vp_bus_out32
        (vp_bus* self, reg_t addr, u32_t* buf)            {
            if (trait_of(self) != vp_bus_t) return false_t;
            if (!buf)                       return false_t;

            if (vp_pa_begin(self->pa) <  addr) return false_t;
            if (vp_pa_end  (self->pa) >= addr) return false_t;
            return self->ops->out32                          (
                self->bus,
                addr     ,
                buf
            );
}

bool_t
    vp_bus_out64
        (vp_bus* self, reg_t addr, u64_t* buf)            {
            if (trait_of(self) != vp_bus_t) return false_t;
            if (!buf)                       return false_t;

            if (vp_pa_begin(self->pa) <  addr) return false_t;
            if (vp_pa_end  (self->pa) >= addr) return false_t;
            return self->ops->out64                          (
                self->bus,
                addr     ,
                buf
            );
}