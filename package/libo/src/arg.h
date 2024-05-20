#ifndef LIBO_CONF_H
#define LIBO_CONF_H

#include <core.h>
#include <fs.h>

struct libo;

extern obj_trait *libo_arg_t;
typedef struct    libo_arg   {
    obj          head;
    file        *arg;
    struct libo *sup;
    u64_t        len;
    u8_t        *ua;
}   libo_arg;

bool_t libo_arg_new   (libo_arg*, u32_t, va_list);
bool_t libo_arg_clone (libo_arg*, libo_arg*)     ;
bool_t libo_arg_del   (libo_arg*)                ;

u64_t  libo_arg_len   (libo_arg*)                ;

#endif
