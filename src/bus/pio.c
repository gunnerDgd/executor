#include "pio.h"
#include "../dev/pio.h"

bool_t
    vp_pio_do_in8
        (vp_pio* pio, reg_t addr, u8_t* in)              {
            if (trait_of(pio) != vp_pio_t) return false_t;
            if (addr >= 64 KB)             return false_t;
            if (!in)                       return false_t;

            node       *nod = map_find (&pio->pio, (any_t) addr);
            vp_pio_dev *dev = value    (nod);

            if (trait_of(dev) != vp_pio_dev_t) return false_t;
            if (trait_of(nod) != node_t)       return false_t;
            return dev->ops->in8 (
                   dev->dev,
                   in
            );
}

bool_t
    vp_pio_do_in16
        (vp_pio* pio, reg_t addr, u16_t* in)             {
            if (trait_of(pio) != vp_pio_t) return false_t;
            if (addr >= 64 KB)             return false_t;
            if (!in)                       return false_t;

            node       *nod = map_find (&pio->pio, (any_t) addr);
            vp_pio_dev *dev = value    (nod);

            if (trait_of(dev) != vp_pio_dev_t) return false_t;
            if (trait_of(nod) != node_t)       return false_t;
            return dev->ops->in16 (
                   dev->dev,
                   in
            );
}

bool_t
    vp_pio_do_in32
        (vp_pio* pio, reg_t addr, u32_t* in)             {
            if (trait_of(pio) != vp_pio_t) return false_t;
            if (addr >= 64 KB)             return false_t;
            if (!in)                       return false_t;

            node       *nod = map_find (&pio->pio, (any_t) addr);
            vp_pio_dev *dev = value    (nod);

            if (trait_of(dev) != vp_pio_dev_t) return false_t;
            if (trait_of(nod) != node_t)       return false_t;
            return dev->ops->in32 (
                   dev->dev,
                   in
            );
}

bool_t
    vp_pio_do_in64
        (vp_pio* pio, reg_t addr, u32_t in) {
            return false_t;
}

bool_t
    vp_pio_do_out8
        (vp_pio* pio, reg_t addr, u8_t* out)             {
            if (trait_of(pio) != vp_pio_t) return false_t;
            if (addr >= 64 KB)             return false_t;
            if (!out)                      return false_t;

            node       *nod = map_find (&pio->pio, (any_t) addr);
            vp_pio_dev *dev = value    (nod);

            if (trait_of(dev) != vp_pio_dev_t) return false_t;
            if (trait_of(nod) != node_t)       return false_t;
            return dev->ops->out8 (
                   dev->dev,
                   out
            );
}

bool_t
    vp_pio_do_out16
        (vp_pio* pio, reg_t addr, u16_t* out)            {
            if (trait_of(pio) != vp_pio_t) return false_t;
            if (addr >= 64 KB)             return false_t;
            if (!out)                      return false_t;

            node       *nod = map_find (&pio->pio, (any_t) addr);
            vp_pio_dev *dev = value    (nod);

            if (trait_of(dev) != vp_pio_dev_t) return false_t;
            if (trait_of(nod) != node_t)       return false_t;
            return dev->ops->out16 (
                   dev->dev,
                   out
            );
}

bool_t
    vp_pio_do_out32
        (vp_pio* pio, reg_t addr, u32_t* out)             {
            if (trait_of(pio) != vp_pio_t) return false_t;
            if (addr >= 64 KB)             return false_t;
            if (!out)                      return false_t;

            node       *nod = map_find (&pio->pio, (any_t) addr);
            vp_pio_dev *dev = value    (nod);

            if (trait_of(dev) != vp_pio_dev_t) return false_t;
            if (trait_of(nod) != node_t)       return false_t;
            return dev->ops->out32 (
                   dev->dev,
                   out
            );
}

bool_t
    vp_pio_do_out64
        (vp_pio* pio, reg_t addr, u32_t out) {
            return false_t;
}

vp_bus_ops vp_pio_do = vp_make_bus_ops (
    vp_pio_do_in8  ,
    vp_pio_do_in16 ,
    vp_pio_do_in32 ,
    vp_pio_do_in64 ,
    vp_pio_do_out8 ,
    vp_pio_do_out16,
    vp_pio_do_out32,
    vp_pio_do_out64
);

obj_trait vp_pio_trait = make_trait (
    vp_pio_new    ,
    vp_pio_clone  ,
    null_t        ,
    vp_pio_del    ,
    sizeof(vp_pio),
    null_t
);

obj_trait *vp_pio_t = &vp_pio_trait;

bool_t
    vp_pio_new
        (vp_pio* self, u32_t count, va_list arg)                                {
            vp_root *root = null_t; if (count > 0) root = va_arg (arg, vp_root*);
            if (!make_at(&self->pa , vp_pa)  from (3, root, 0, 64 KB))              goto new_err;
            if (!make_at(&self->bus, vp_bus) from (3, &vp_pio_do, self, &self->pa)) goto new_err;
            if (!make_at(&self->pio, map)    from (0))                              goto new_err;
            return true_t;
    new_err:
            del (&self->pio);
            del (&self->bus);
            del (&self->pa) ;
            return false_t;

}

bool_t
    vp_pio_clone
        (vp_pio* self, vp_pio* clone) {
            return false_t;
}

void
    vp_pio_del
        (vp_pio* self)      {
            del (&self->pio);
            del (&self->bus);
            del (&self->pa) ;
}