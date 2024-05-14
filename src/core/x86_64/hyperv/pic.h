#ifndef VAPOR_CORE_HYPERV_PIC_H
#define VAPOR_CORE_HYPERV_PIC_H

#include <core.h>
#include <collections.h>

struct vp_root;
struct vp_irq;

extern obj_trait *vp_pic_t;
typedef struct    vp_pic  {
    obj             head;
    struct vp_root* root;
    u64_t           num;
    map             irq;
}   vp_pic;

bool_t         vp_pic_new  (vp_pic*, u32_t, va_list);
bool_t         vp_pic_clone(vp_pic*, vp_pic*)       ;
void           vp_pic_del  (vp_pic*)                ;

struct vp_irq* vp_pic_irq  (vp_pic*, u64_t);

#endif