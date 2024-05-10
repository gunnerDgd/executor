#ifndef VAPOR_DEV_PIO_H
#define VAPOR_DEV_PIO_H

#include "../bus.h"

typedef struct vp_pio_ops               {
    bool_t (*out32)(obj*, u16_t, u32_t*);
    bool_t (*out16)(obj*, u16_t, u16_t*);
    bool_t (*out8) (obj*, u16_t, u8_t *);

    bool_t (*in32) (obj*, u16_t, u32_t*);
    bool_t (*in16) (obj*, u16_t, u16_t*);
    bool_t (*in8)  (obj*, u16_t, u8_t *);
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
typedef struct    vp_pio_dev  {
    obj         head;
    u64_t       begin, end;
    vp_pio_ops *ops;
    vp_pio     *pio;
    obj        *dev;
}   vp_pio_dev;

bool_t vp_pio_dev_new  (vp_pio_dev*, u32_t, va_list);
bool_t vp_pio_dev_clone(vp_pio_dev*, vp_pio_dev*)   ;
void   vp_pio_dev_del  (vp_pio_dev*)                ;

#endif
