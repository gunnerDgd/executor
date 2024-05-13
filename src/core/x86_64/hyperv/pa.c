#include "pa.h"

#include "root.h"
#include "cpu.h"

ord_t
    vp_pa_do_ord
        (vp_pa* self, vp_pa* arg)                        {
            if (trait_of(self) != vp_pa_t) return ord_err;
            if (trait_of(arg)  != vp_pa_t) return ord_err;

            if (vp_pa_end  (self) < vp_pa_begin(arg)) return ord_lt;
            if (vp_pa_begin(self) > vp_pa_end  (arg)) return ord_gt;
            return ord_eq;
}

ord_t
    vp_pa_do_ord_arg
        (vp_pa* self, reg_t arg)                         {
            if (trait_of(self) != vp_pa_t) return ord_err;
            if (vp_pa_begin(self) > arg)   return ord_gt;
            if (vp_pa_end  (self) < arg)   return ord_lt;
            return ord_eq;
}

ops_cmp vp_pa_do_cmp = make_ops_cmp (
    vp_pa_do_ord_arg,
    vp_pa_do_ord
);

obj_ops vp_pa_do      = {
    .cmp = &vp_pa_do_cmp
};

obj_trait vp_pa_trait = make_trait (
    vp_pa_new    ,
    vp_pa_clone  ,
    null_t       ,
    vp_pa_del    ,
    sizeof(vp_pa),
    &vp_pa_do
);

obj_trait *vp_pa_t = &vp_pa_trait;

bool_t
    vp_pa_new
        (vp_pa* self, u32_t count, va_list arg)                             {
            vp_root *root = null_t; if (count > 0) root = va_arg(arg, any_t);
            reg_t    pa   = 0ull  ; if (count > 1) pa   = va_arg(arg, reg_t);
            u64_t    len  = 0ull  ; if (count > 2) len  = va_arg(arg, u64_t);
            vp_cpu  *cpu  = null_t; if (count > 3) cpu  = va_arg(arg, any_t);

            if (trait_of(root) != vp_root_t) return false_t;
            if (len & mask (11))             return false_t;
            if (!len)                        return false_t;

            self->root = root;
            self->cpu  = cpu;
            self->len  = len;
            self->pa   = pa;

            if (!map_move(&root->pa, (obj*) self)) return false_t;
            self->sub = null_t;
            return true_t;
}

bool_t vp_pa_clone(vp_pa* self, vp_pa* clone) { return false_t; }
void   vp_pa_del  (vp_pa* self)               { del(self->sub); }

reg_t
    vp_pa_begin
        (vp_pa* self)                               {
            if (trait_of(self) != vp_pa_t) return -1;
            return self->pa;
}

reg_t
    vp_pa_end
        (vp_pa* self)                              {
            if (trait_of(self) != vp_pa_t) return 0;
            return self->pa + self->len;
}

u64_t
    vp_pa_len
        (vp_pa* self)                              {
            if (trait_of(self) != vp_pa_t) return 0;
            return self->len;
}

obj*
    vp_pa_sub
        (vp_pa* self)                                   {
            if (trait_of(self) != vp_pa_t) return null_t;
            return self->sub;
}