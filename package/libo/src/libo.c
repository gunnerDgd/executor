#include "libo.h"
#include <asm/bootparam.h>

obj_trait libo_trait = make_trait (
    libo_new    ,
    libo_clone  ,
    null_t      ,
    libo_del    ,
    sizeof(libo),
    null_t
);

obj_trait *libo_t = &libo_trait;

bool_t
    libo_new
        (libo* self, u32_t count, va_list arg)                              {
            vp_root *root = null_t; if (count > 0) root = va_arg(arg, any_t);
            file    *ker  = null_t; if (count > 1) ker  = va_arg(arg, any_t);
            file    *ima  = null_t; if (count > 2) ima  = va_arg(arg, any_t);
            if (trait_of(root) != vp_root_t) return false_t;
            if (trait_of(ker)  != file_t)    return false_t;
            self->root = root;

            if (!make_at (&self->arg, libo_arg) from (2, self, ker)) return false_t;
            if (!make_at (&self->ker, libo_ker) from (2, self, ker)) return false_t;

            make_at (&self->ima, libo_ima) from (2, self, ima);
            self->conf->loadflags |= 0x01 | KEEP_SEGMENTS;
            return true_t;
}

bool_t libo_clone (libo* self, libo* clone) { return false_t; }
void   libo_del   (libo* self)              {                 }

void   libo_ima_pos(libo* self, u64_t arg) { if (trait_of(self) == libo_t) self->conf->ramdisk_image = arg; }
void   libo_ima_len(libo* self, u64_t arg) { if (trait_of(self) == libo_t) self->conf->ramdisk_size  = arg; }

void   libo_video     (libo* self, u64_t arg) { if (trait_of(self) == libo_t) self->conf->vid_mode       = arg; }
void   libo_loader_ver(libo* self, u64_t arg) { if (trait_of(self) == libo_t) self->conf->ext_loader_ver = arg; }
void   libo_loader    (libo* self, u64_t arg) { if (trait_of(self) == libo_t) self->conf->type_of_loader = arg; }

void
    libo_command
        (libo* self, const char* com, u64_t len) {
            if (trait_of(self) != libo_t) return;
            if (!com)                     return;
            if (!len)                     return;

            mem_copy (vp_root_ua (self->root, 128 KB, len), com, len);
            self->conf->cmd_line_ptr = 128 KB;
            self->conf->cmdline_size = len;
}

void
    libo_mem
        (libo* self, vp_map* mem)                 {
            if (trait_of(mem)  != vp_map_t) return;
            if (trait_of(self) != libo_t)   return;
            u64_t  num = self->boot->e820_entries;
            vp_pa *pa  = vp_map_pa(mem);

            self->boot->e820_entries = num + 1;
            self->mem[num].addr      = vp_pa_begin(pa);
            self->mem[num].size      = vp_pa_len  (pa);
            self->mem[num].type      = 1;
}