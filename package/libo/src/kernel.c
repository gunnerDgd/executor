#include "kernel.h"
#include "libo.h"

obj_trait libo_ker_trait = make_trait (
    libo_ker_new    ,
    libo_ker_clone  ,
    null_t          ,
    libo_ker_del    ,
    sizeof(libo_ker),
    null_t
);

obj_trait *libo_ker_t = &libo_ker_trait;

bool_t
    libo_ker_new
        (libo_ker* self, u32_t count, va_list arg)                     {
            libo *sup = null_t; if (count > 0) sup = va_arg(arg, any_t);
            file *ker = null_t; if (count > 1) ker = va_arg(arg, any_t);

            if (trait_of(ker) != file_t) return false_t;
            if (trait_of(sup) != libo_t) return false_t;
            u64_t len;
            u8_t* ua;

            len = file_size(ker) - libo_arg_len(&sup->arg);
            ua  = vp_root_ua                              (
                sup->root,
                1 MB     ,
                self->len
            );

            if (await (file_read (ker, ua, len)) != len) return false_t;
            if (!ua)                                     return false_t;
            self->ker = ker;
            self->sup = sup;
            self->len = len;
            self->ua  = ua;
            return true_t;
}

bool_t libo_ker_clone(libo_ker* self, libo_ker* clone) { return false_t; }
void   libo_ker_del  (libo_ker* self)                  {                 }