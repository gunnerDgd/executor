#ifndef VAPOR_DEV_PIO_H
#define VAPOR_DEV_PIO_H

#include <core.h>

typedef struct vp_pio_ops        {
    bool_t (*out32)(obj*, u32_t*);
    bool_t (*out16)(obj*, u16_t*);
    bool_t (*out8) (obj*, u8_t *);

    bool_t (*in32) (obj*, u32_t*);
    bool_t (*in16) (obj*, u16_t*);
    bool_t (*in8)  (obj*, u8_t *);
}   vp_pio_ops;

#define vp_make_pio_ops(par_in8, par_in16, par_in32, par_out8, par_out16, par_out32) {\
    .in8   = (bool_t(*)(obj*, reg_t, u8_t *))(par_in8)  ,                             \
    .in16  = (bool_t(*)(obj*, reg_t, u16_t*))(par_in16) ,                             \
    .in32  = (bool_t(*)(obj*, reg_t, u32_t*))(par_in32) ,                             \
                                                                                      \
    .out8  = (bool_t(*)(obj*, reg_t, u8_t *))(par_out8) ,                             \
    .out16 = (bool_t(*)(obj*, reg_t, u16_t*))(par_out16),                             \
    .out32 = (bool_t(*)(obj*, reg_t, u32_t*))(par_out32),                             \
}

extern obj_trait *vp_pio_dev_t;
typedef struct    vp_pio_dev { u8_t pio_dev[128]; } vp_pio_dev;

#endif
