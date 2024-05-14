#ifndef VAPOR_CORE_PIC_H
#define VAPOR_CORE_PIC_H

#include <core.h>

struct vp_irq;

extern obj_trait* vp_pic_t;
typedef struct    vp_pic { u8_t pic[1 KB]; } vp_pic;

struct vp_irq* vp_pic_irq(vp_pic*, u64_t);

#endif
