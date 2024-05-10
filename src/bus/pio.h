#ifndef VAPOR_BUS_PIO_H
#define VAPOR_BUS_PIO_H

#include "../core.h"

extern obj_trait *vp_pio_t;
typedef struct    vp_pio  {
    obj    head;
    map    pio;
    vp_bus bus;
    vp_pa  pa;
}   vp_pio;

bool_t vp_pio_new  (vp_pio*, u32_t, va_list);
bool_t vp_pio_clone(vp_pio*, vp_pio*)       ;
void   vp_pio_del  (vp_pio*)                ;

#endif
