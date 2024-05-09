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

#endif
