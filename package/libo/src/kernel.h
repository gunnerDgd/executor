#ifndef LIBO_KERNEL_H
#define LIBO_KERNEL_H

#include <core.h>
#include <fs.h>

struct libo;

extern obj_trait *libo_ker_t;
typedef struct    libo_ker  {
    obj          head;
    struct libo *sup;
    file        *ker;
    u64_t        len;
    u8_t        *ua;
}   libo_ker;

bool_t libo_ker_new  (libo_ker*, u32_t, va_list);
bool_t libo_ker_clone(libo_ker*, libo_ker*)     ;
void   libo_ker_del  (libo_ker*)                ;

#endif