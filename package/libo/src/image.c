#include "image.h"
#include "libo.h"

obj_trait libo_ima_trait = make_trait (
    libo_ima_new    ,
    libo_ima_clone  ,
    null_t          ,
    libo_ima_del    ,
    sizeof(libo_ima),
    null_t
);

obj_trait *libo_ima_t = &libo_ima_trait;

bool_t
    libo_ima_new
        (libo_ima* self, u32_t count, va_list arg)                     {
            libo *sup = null_t; if (count > 0) sup = va_arg(arg, any_t);
            file *ima = null_t; if (count > 1) ima = va_arg(arg, any_t);

            if (trait_of(ima) != file_t) return false_t;
            if (trait_of(sup) != libo_t) return false_t;
            self->len = file_size(ima);
            self->ua  = vp_root_ua    (
                sup->root,
                64 MB    ,
                self->len
            );

            if (await (file_read (ima, self->ua, self->len)) != self->len) return false_t;
            if (!self->ua)                                                 return false_t;
            libo_ima_len (sup, self->len);
            libo_ima_pos (sup, 64 MB);

            self->ima = ima;
            self->sup = sup;
            return true_t;
}

bool_t libo_ima_clone(libo_ima* self, libo_ima* clone) { return false_t; }
void   libo_ima_del  (libo_ima* self)                  {                 }