#ifndef VAPOR_CORE_KVM_BUS_H
#define VAPOR_CORE_KVM_BUS_H

#include <core.h>

struct vp_pa;

typedef struct vp_bus_ops               {
    bool_t (*out64)(obj*, reg_t, u64_t*);
    bool_t (*out32)(obj*, reg_t, u32_t*);
    bool_t (*out16)(obj*, reg_t, u16_t*);
    bool_t (*out8) (obj*, reg_t, u8_t *);

    bool_t (*in64) (obj*, reg_t, u64_t*);
    bool_t (*in32) (obj*, reg_t, u32_t*);
    bool_t (*in16) (obj*, reg_t, u16_t*);
    bool_t (*in8)  (obj*, reg_t, u8_t *);
}   vp_bus_ops;

#define vp_make_bus_ops(par_in8, par_in16, par_in32, par_in64, par_out8, par_out16, par_out32, par_out64) { \
    .in8   = (bool_t(*)(obj*, reg_t, u8_t *))(par_in8)  ,                                                   \
    .in16  = (bool_t(*)(obj*, reg_t, u16_t*))(par_in16) ,                                                   \
    .in32  = (bool_t(*)(obj*, reg_t, u32_t*))(par_in32) ,                                                   \
    .in64  = (bool_t(*)(obj*, reg_t, u64_t*))(par_in64) ,                                                   \
                                                                                                            \
    .out8  = (bool_t(*)(obj*, reg_t, u8_t *))(par_out8) ,                                                   \
    .out16 = (bool_t(*)(obj*, reg_t, u16_t*))(par_out16),                                                   \
    .out32 = (bool_t(*)(obj*, reg_t, u32_t*))(par_out32),                                                   \
    .out64 = (bool_t(*)(obj*, reg_t, u64_t*))(par_out64),                                                   \
}

extern obj_trait *vp_bus_t;
typedef struct    vp_bus  {
    obj           head;
    vp_bus_ops   *ops;
    obj          *bus;
    struct vp_pa *pa;
}   vp_bus;

bool_t vp_bus_new  (vp_bus*, u32_t, va_list);
bool_t vp_bus_clone(vp_bus*, vp_bus*)       ;
void   vp_bus_del  (vp_bus*)                ;

bool_t vp_bus_in8  (vp_bus*, reg_t, u8_t *);
bool_t vp_bus_in16 (vp_bus*, reg_t, u16_t*);
bool_t vp_bus_in32 (vp_bus*, reg_t, u32_t*);
bool_t vp_bus_in64 (vp_bus*, reg_t, u64_t*);

bool_t vp_bus_out8 (vp_bus*, reg_t, u8_t *);
bool_t vp_bus_out16(vp_bus*, reg_t, u16_t*);
bool_t vp_bus_out32(vp_bus*, reg_t, u32_t*);
bool_t vp_bus_out64(vp_bus*, reg_t, u64_t*);

#endif
