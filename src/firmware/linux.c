#include "linux.h"
#include "../firmware.h"

bool_t
    linux_firm_do_prep_root
        (linux_firm* self, vp_root* root)                     {
            if (trait_of(self) != linux_firm_t) return false_t;
            if (trait_of(root) != vp_root_t)    return false_t;

            vp_pa *firm = make (vp_pa) from (3, root, 64 KB, 1  MB - 64 KB);
            vp_pa *kern = make (vp_pa) from (3, root, 1  MB, 64 MB);
            vp_pa *ram  = make (vp_pa) from (3, root, 65 MB, 64 MB);

            if (!make (vp_map) from (3, firm, mem_acq(self->firm, null_t, 1  MB - 64 KB))) goto prep_err;
            if (!make (vp_map) from (3, kern, mem_acq(self->kern, null_t, 64 MB)))         goto prep_err;
            if (!make (vp_map) from (3, ram , mem_acq(self->ram , null_t, 64 MB)))         goto prep_err;
            return true_t;
    prep_err:
            del     (firm);
            del     (kern);
            del     (ram) ;
            return false_t;
}

bool_t
    linux_firm_do_prep_cpu
        (linux_firm* self, vp_cpu* cpu)                       {
            if (trait_of(self) != linux_firm_t) return false_t;
            if (trait_of(cpu)  != vp_cpu_t)     return false_t;
            vp_reg reg;

            if (!vp_cpu_sync_out(cpu, &reg)) return false_t;
            reg.cs.base  =  0;
            reg.cs.limit = ~0;
            reg.cs.g     =  1;
            reg.cs.db    =  1;

            reg.ss.base  =  0;
            reg.ss.limit = ~0;
            reg.ss.g     =  1;
            reg.ss.db    =  1;

            reg.ds.base  =  0;
            reg.ds.limit = ~0;
            reg.ds.g     =  1;

            reg.fs.base  =  0;
            reg.fs.limit = ~0;
            reg.fs.g     =  1;

            reg.gs.base  =  0;
            reg.gs.limit = ~0;
            reg.gs.g     =  1;

            reg.es.base  =  0;
            reg.es.limit = ~0;
            reg.es.g     =  1;

            reg.gen.rflags = 0x02;
            reg.gen.rip    = 1  MB;
            reg.gen.rsi    = 64 KB;

            return vp_cpu_sync_in (cpu, &reg);
}

vp_firm_ops linux_firm_do = vp_make_firm_ops (
    linux_firm_do_prep_root,
    linux_firm_do_prep_cpu
);

obj*
    linux_firm_do_as
        (obj* self, obj_trait* trait)                        {
            if (trait_of(self) != linux_firm_t) return null_t;
            if (trait != vp_firm_t)             return null_t;
            return make (vp_firm) from                       (
                2            ,
                linux_firm_do,
                self
            );
}

ops_cast linux_firm_ops_cast = { .as   = linux_firm_do_as };
obj_ops  linux_firm_ops      = { .cast = &linux_firm_ops_cast };

obj_trait linux_firm_trait = make_trait (
    linux_firm_new    ,
    linux_firm_clone  ,
    null_t            ,
    linux_firm_del    ,
    sizeof(linux_firm),
    &linux_firm_ops
);

obj_trait *linux_firm_t = &linux_firm_trait;

bool_t
    linux_firm_new
        (linux_firm* self, u32_t count, va_list arg)                     {
            file *firm = null_t; if (count > 0) firm = va_arg(arg, any_t);
            file *kern = null_t; if (count > 1) kern = va_arg(arg, any_t);
            file *ram  = null_t; if (count > 2) ram  = va_arg(arg, any_t);

            if (trait_of(firm) != file_t) return false_t;
            if (trait_of(kern) != file_t) return false_t;
            if (trait_of(ram)  != file_t) return false_t;

            self->firm = make (mem) from (2, file_map, firm);
            self->kern = make (mem) from (2, file_map, kern);
            self->ram  = make (mem) from (2, file_map, ram) ;

            if (trait_of(self->firm) != mem_t) goto new_err;
            if (trait_of(self->kern) != mem_t) goto new_err;
            if (trait_of(self->ram)  != mem_t) goto new_err;
            u8_t* par = null_t;

            if (!(par = mem_acq(self->firm, null_t, 1 MB - 64 KB))) goto new_err;
            par      += 0x1f1;
            self->arg = par  ;

            self->arg->ramdisk_image = 65  MB;
            self->arg->ramdisk_size  = 64  MB;
            self->arg->cmd_line_ptr  = 128 KB;
            return true_t;
    new_err:
            del (self->firm);
            del (self->kern);
            del (self->ram) ;
            return false_t;
}

bool_t
    linux_firm_clone
        (linux_firm* self, linux_firm* clone) {
            return false_t;
}

void
    linux_firm_del
        (linux_firm* self)  {
            del (self->firm);
            del (self->kern);
            del (self->ram) ;
}

void
    linux_par_cstr
        (linux_firm* self, const char* par, u64_t len) {
            if (trait_of(self) != linux_firm_t) return;
            if (!par)                           return;
            if (!len)                           return;

            u8_t    *buf = (u8_t*)self->arg + 64 KB;
            mem_copy(buf, par, len);

            self->arg->cmd_line_ptr = 128 KB;
            self->arg->cmdline_size = len;
}
void
    linux_par
        (linux_firm* self, str* par)                  {
            if (trait_of(self) != linux_firm_t) return;
            if (trait_of(par)  != str_t)        return;
            linux_par_cstr                            (
                self        ,
                str_ptr(par),
                str_len(par)
            );
}

void linux_loader_flag(linux_firm* self, u64_t arg) { if (trait_of(self) == linux_firm_t) self->arg->loadflags      = arg; }
void linux_loader_ver (linux_firm* self, u64_t arg) { if (trait_of(self) == linux_firm_t) self->arg->ext_loader_ver = arg; }
void linux_loader     (linux_firm* self, u64_t arg) { if (trait_of(self) == linux_firm_t) self->arg->type_of_loader = arg; }
void linux_video      (linux_firm* self, u64_t arg) { if (trait_of(self) == linux_firm_t) self->arg->vid_mode       = arg; }
void linux_heap       (linux_firm* self, u64_t arg) { if (trait_of(self) == linux_firm_t) self->arg->heap_end_ptr   = arg; }