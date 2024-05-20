#include "arg.h"
#include "libo.h"

#include <asm/bootparam.h>

obj_trait libo_arg_trait = make_trait (
    libo_arg_new    ,
    libo_arg_clone  ,
    null_t          ,
    libo_arg_del    ,
    sizeof(libo_arg),
    null_t
);

obj_trait *libo_arg_t = &libo_arg_trait;

bool_t
    libo_arg_new
        (libo_arg* self, u32_t count, va_list arg)                     {
            libo* sup = null_t; if (count > 0) sup = va_arg(arg, any_t);
            file* par = null_t; if (count > 1) par = va_arg(arg, any_t);

            if (trait_of(par) != file_t) return false_t;
            if (trait_of(sup) != libo_t) return false_t;

            self->ua  = vp_root_ua (sup->root, 64 KB, 4 KB);
            self->arg = par;
            self->sup = sup;

            if (await (file_read(par, self->ua, 4 KB)) != 4 KB) return false_t;
            if (!self->ua)                                      return false_t;
            sup->boot  = self->ua;
            sup->video = &sup->boot->screen_info  ;
            sup->conf  = &sup->boot->hdr          ;
            sup->apm   = &sup->boot->apm_bios_info;
            sup->ist   = &sup->boot->ist_info     ;
            sup->efi   = &sup->boot->efi_info     ;
            sup->mem   =  sup->boot->e820_table   ;

            self->len = sup->conf->setup_sects;
            self->len = shl (self->len + 1, 9);

            file_pos (par, self->len);
            return true_t;
}

bool_t libo_arg_clone(libo_arg* self, libo_arg* clone) { return false_t; }
bool_t libo_arg_del  (libo_arg* self)                  {                 }

u64_t
    libo_arg_len
        (libo_arg* self)                              {
            if (trait_of(self) != libo_arg_t) return 0;
            return self->len;
}