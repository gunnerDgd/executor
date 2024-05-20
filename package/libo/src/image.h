#ifndef LIBO_IMAGE_H
#define LIBO_IMAGE_H

#include <core.h>
#include <fs.h>

struct libo;

extern obj_trait *libo_ima_t;
typedef struct    libo_ima  {
    obj          head;
    struct libo *sup;
    file        *ima;
    u64_t        len;
    u8_t        *ua;
}   libo_ima;

bool_t libo_ima_new  (libo_ima*, u32_t, va_list);
bool_t libo_ima_clone(libo_ima*, libo_ima*)     ;
void   libo_ima_del  (libo_ima*)                ;

#endif
