#include "cpu.h"

#include "../kvm.h"
#include "root.h"
#include "pa.h"
#include "va.h"


ord_t
    vp_cpu_do_ord
        (vp_cpu *self, vp_cpu *arg)                       {
            if (trait_of(self) != vp_cpu_t) return ord_err;
            if (trait_of(arg)  != vp_cpu_t) return ord_err;

            if (self->cpu < arg->cpu) return ord_lt;
            if (self->cpu > arg->cpu) return ord_gt;
            return ord_eq;
}

ord_t
    vp_cpu_do_ord_arg
        (vp_cpu *self, u64_t arg)                         {
            if (trait_of(self) != vp_cpu_t) return ord_err;
            if (trait_of(arg)  != vp_cpu_t) return ord_err;

            if (self->cpu < arg) return ord_lt;
            if (self->cpu > arg) return ord_gt;
            return ord_eq;
}

ops_cmp vp_cpu_do_cmp = make_ops_cmp (
    vp_cpu_do_ord_arg,
    vp_cpu_do_ord
);

obj_ops vp_cpu_do      = {
    .cmp = &vp_cpu_do_cmp
};


obj_trait vp_cpu_trait = make_trait (
    vp_cpu_new    ,
    vp_cpu_clone  ,
    null_t        ,
    vp_cpu_del    ,
    sizeof(vp_cpu),
    &vp_cpu_do
);

obj_trait *vp_cpu_t = &vp_cpu_trait;

bool_t
    vp_cpu_new
        (vp_cpu* self, u32_t count, va_list arg)                            {
            vp_root *root = null_t; if (count > 0) root = va_arg(arg, any_t);
            u64_t    cpu  = 0ull  ; if (count > 1) cpu  = va_arg(arg, u64_t);
            if (trait_of(root) != vp_root_t) return false_t;
            self->root = root;
            self->cpu  = cpu ;

            if (WHvCreateVirtualProcessor(root->root, cpu, 0)) return false_t;
            if (!map_move (&root->cpu, self))                                {
                WHvDeleteVirtualProcessor(root->root, cpu);
                return false_t;
            }

            return true_t;
}

bool_t
    vp_cpu_clone
        (vp_cpu* self, vp_cpu* clone) {
            return false_t;
}

void
    vp_cpu_del
        (vp_cpu* self)                                            {
            WHvDeleteVirtualProcessor(self->root->root, self->cpu);
}

static WHV_REGISTER_NAME vp_cpu_sync[] = {
    WHvX64RegisterRax, // Index 0
    WHvX64RegisterRbx, // Index 1
    WHvX64RegisterRcx, // Index 2
    WHvX64RegisterRdx, // Index 3

    WHvX64RegisterRsp, // Index 4
    WHvX64RegisterRbp, // Index 5
    WHvX64RegisterRsi, // Index 6
    WHvX64RegisterRdi, // Index 7

    WHvX64RegisterR8 , // Index 8
    WHvX64RegisterR9 , // Index 9
    WHvX64RegisterR10, // Index 10
    WHvX64RegisterR11, // Index 11
    WHvX64RegisterR12, // Index 12
    WHvX64RegisterR13, // Index 13
    WHvX64RegisterR14, // Index 14
    WHvX64RegisterR15, // Index 15

    WHvX64RegisterRip   , // Index 16
    WHvX64RegisterRflags, // Index 17

    WHvX64RegisterCs,   // Index 18
    WHvX64RegisterDs,   // Index 19
    WHvX64RegisterEs,   // Index 20
    WHvX64RegisterSs,   // Index 21
    WHvX64RegisterGs,   // Index 22
    WHvX64RegisterFs,   // Index 23
    WHvX64RegisterLdtr, // Index 24

    WHvX64RegisterIdtr, // Index 25
    WHvX64RegisterGdtr  // Index 26
};

#define vp_desc_sync_out(desc, arg)\
    desc.base  = arg.Table.Base ;  \
    desc.limit = arg.Table.Limit;  \

#define vp_desc_sync_in(desc, arg)\
    desc.Table.Base  = arg.base;  \
    desc.Table.Limit = arg.limit; \

#define vp_seg_sync_out(seg, arg)                       \
    seg.base     = arg.Segment.Base;                    \
    seg.limit    = arg.Segment.Limit;                   \
    seg.selector = arg.Segment.Selector;                \
    seg.type     = arg.Segment.SegmentType;             \
    seg.present  = arg.Segment.Present;                 \
    seg.dpl      = arg.Segment.DescriptorPrivilegeLevel;\
    seg.db       = arg.Segment.Default;                 \
    seg.s        = arg.Segment.NonSystemSegment;        \
    seg.l        = arg.Segment.Long;                    \
    seg.g        = arg.Segment.Granularity;             \
    seg.avl      = arg.Segment.Available;               \

#define vp_seg_sync_in(seg, arg)                        \
    seg.Segment.Base                     = arg.base;    \
    seg.Segment.Limit                    = arg.limit;   \
    seg.Segment.Selector                 = arg.selector;\
    seg.Segment.SegmentType              = arg.type;    \
    seg.Segment.Present                  = arg.present; \
    seg.Segment.DescriptorPrivilegeLevel = arg.dpl;     \
    seg.Segment.Default                  = arg.db;      \
    seg.Segment.NonSystemSegment         = arg.s;       \
    seg.Segment.Long                     = arg.l;       \
    seg.Segment.Granularity              = arg.g;       \
    seg.Segment.Available                = arg.avl;     \

#define vp_gen_sync_out(gen, arg)\
    gen.rflags = arg[17].Reg64;\
    gen.rax    = arg[0] .Reg64;\
    gen.rbx    = arg[1] .Reg64;\
    gen.rcx    = arg[2] .Reg64;\
    gen.rdx    = arg[3] .Reg64;\
    gen.r8     = arg[8] .Reg64;\
    gen.r9     = arg[9] .Reg64;\
    gen.r10    = arg[10].Reg64;\
    gen.r11    = arg[11].Reg64;\
    gen.r12    = arg[12].Reg64;\
    gen.r13    = arg[13].Reg64;\
    gen.r14    = arg[14].Reg64;\
    gen.r15    = arg[15].Reg64;\
    gen.rsp    = arg[4] .Reg64;\
    gen.rbp    = arg[5] .Reg64;\
    gen.rsi    = arg[6] .Reg64;\
    gen.rdi    = arg[7] .Reg64;\
    gen.rip    = arg[16].Reg64;\

#define vp_gen_sync_in(gen, arg)\
    gen[17].Reg64 = arg.rflags;  \
    gen[0] .Reg64 = arg.rax;     \
    gen[1] .Reg64 = arg.rbx;     \
    gen[2] .Reg64 = arg.rcx;     \
    gen[3] .Reg64 = arg.rdx;     \
    gen[8] .Reg64 = arg.r8;      \
    gen[9] .Reg64 = arg.r9;      \
    gen[10].Reg64 = arg.r10;     \
    gen[11].Reg64 = arg.r11;     \
    gen[12].Reg64 = arg.r12;     \
    gen[13].Reg64 = arg.r13;     \
    gen[14].Reg64 = arg.r14;     \
    gen[15].Reg64 = arg.r15;     \
    gen[4] .Reg64 = arg.rsp;     \
    gen[5] .Reg64 = arg.rbp;     \
    gen[6] .Reg64 = arg.rsi;     \
    gen[7] .Reg64 = arg.rdi;     \
    gen[16].Reg64 = arg.rip;     \

bool_t
    vp_cpu_sync_out
        (vp_cpu* self, vp_reg* arg)                       {
            if (trait_of(self) != vp_cpu_t) return false_t;
            if (!arg)                       return false_t;
            WHV_REGISTER_VALUE reg[27];
            HRESULT            res = WHvGetVirtualProcessorRegisters (
                self->root->root,
                self->cpu       ,
                &vp_cpu_sync    , 
                27              ,
                reg
            );

            if (res) return false_t;
            vp_desc_sync_out(arg->gdt, reg[25]);
            vp_desc_sync_out(arg->idt, reg[26]);
            vp_seg_sync_out (arg->cs , reg[18]);
            vp_seg_sync_out (arg->ds , reg[19]);
            vp_seg_sync_out (arg->es , reg[20]);
            vp_seg_sync_out (arg->ss , reg[21]);
            vp_seg_sync_out (arg->gs , reg[22]);
            vp_seg_sync_out (arg->fs , reg[23]);
            vp_seg_sync_out (arg->ldt, reg[24]);
            vp_gen_sync_out (arg->gen, reg);
            return true_t;
}

bool_t
    vp_cpu_sync_in
        (vp_cpu* self, vp_reg* arg)                       {
            if (trait_of(self) != vp_cpu_t) return false_t;
            if (!arg)                       return false_t;
            WHV_REGISTER_VALUE reg[27];
            
            vp_desc_sync_in(reg[25], arg->gdt);
            vp_desc_sync_in(reg[26], arg->idt);
            vp_seg_sync_in (reg[18], arg->cs) ;
            vp_seg_sync_in (reg[19], arg->ds) ;
            vp_seg_sync_in (reg[20], arg->es) ;
            vp_seg_sync_in (reg[21], arg->ss) ;
            vp_seg_sync_in (reg[22], arg->gs) ;
            vp_seg_sync_in (reg[23], arg->fs) ;
            vp_seg_sync_in (reg[24], arg->ldt);
            vp_gen_sync_in (reg    , arg->gen);

            HRESULT res = WHvGetVirtualProcessorRegisters (
                self->root->root,
                self->cpu       ,
                &vp_cpu_sync    , 
                27              ,
                reg
            );

            if (res) return false_t;
            return true_t;
}