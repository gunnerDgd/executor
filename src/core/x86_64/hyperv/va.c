#include "va.h"

#include "root.h"
#include "cpu.h"
#include "pa.h"

obj_trait vp_va_trait = make_trait (
    vp_va_new    ,
    vp_va_clone  ,
    null_t       ,
    vp_va_del    ,
    sizeof(vp_va),
    null_t
);

obj_trait *vp_va_t = &vp_va_trait;

bool_t
    vp_va_new
        (vp_va* self, u32_t count, va_list arg)                                 {
            struct vp_cpu *cpu = null_t; if (count > 0) cpu = va_arg(arg, any_t);
            reg_t          va  = 0ull  ; if (count > 1) va  = va_arg(arg, reg_t);
            u64_t          len = 0ull  ; if (count > 2) len = va_arg(arg, u64_t);

            if (trait_of(cpu) != vp_cpu_t) return false_t;
            self->len = len;
            self->cpu = cpu;
            self->va  = va ;

            WHV_TRANSLATE_GVA_RESULT ret;
            u64_t                    pa ;

            HRESULT res = WHvTranslateGva (
                cpu->root->root,
                cpu->cpu       ,
                va             ,
                7              ,
                &ret           ,
                &pa
            );

            self->pa = vp_root_pa(cpu->root, pa, len);

            if (trait_of(self->pa) != vp_pa_t) return false_t;
            if (res)                           return false_t;
            return true_t;
}

bool_t vp_va_clone (vp_va* self, vp_va* clone) { return false_t; }
void   vp_va_del   (vp_va* self)               {                 }

reg_t
    vp_va_begin
        (vp_va* self)                               {
            if (trait_of(self) != vp_va_t) return -1;
            return self->va;
}

reg_t
    vp_va_end
        (vp_va* self)                              {
            if (trait_of(self) != vp_va_t) return 0;
            return self->va + self->len;
}

u64_t
    vp_va_len
        (vp_va* self)                              {
            if (trait_of(self) != vp_va_t) return 0;
            return self->len;
}

struct vp_pa*
    vp_va_pa
        (vp_va* self)                                   {
            if (trait_of(self) != vp_va_t) return null_t;
            return self->pa;
}
