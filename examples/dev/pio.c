#include <core.h>
#include <thread.h>
#include <io.h>

#include <vp/core.h>
#include <vp/bus.h>
#include <vp/dev.h>

#include <stdio.h>

use (dep(vp_core), dep (io), dep (thread))

bool_t in8  (obj* self, u8_t * buf) { printf("in8 : %d\n"  , *buf); return true_t; }
bool_t in16 (obj* self, u16_t* buf) { printf("in16 : %d\n" , *buf); return true_t; }
bool_t in32 (obj* self, u32_t* buf) { printf("in32 : %d\n" , *buf); return true_t; }
bool_t out8 (obj* self, u8_t * buf) { printf("out8 : %d\n" , *buf); return true_t; }
bool_t out16(obj* self, u16_t* buf) { printf("out16 : %d\n", *buf); return true_t; }
bool_t out32(obj* self, u32_t* buf) { printf("out32 : %d\n", *buf); return true_t; }

vp_pio_ops ops = vp_make_pio_ops (
    in8  ,
    in16 ,
    in32 ,
    out8 ,
    out16,
    out32
);

const u8_t code[]     = {
        0xba, 0xf8, 0x03, /* mov $0x3f8, %dx */
        0x00, 0xd8,       /* add %bl, %al    */
        0x04, '0',        /* add $'0', %al   */
        0xee,             /* out %al, (%dx)  */
        0xb0, '\n',       /* mov $'\n', %al  */
        0xee,             /* out %al, (%dx)  */
        0xf4,             /* hlt */
};

run_async()                                {
    vp_root *root = make (vp_root) from (0);
    vp_pa   *pa   = make (vp_pa)   from (3, root, 1 MB, 4 KB);
    vp_cpu  *cpu  = make (vp_cpu)  from (2, root, 0);

    vp_pio *pio = make (vp_pio) from (1, root);
    if (!make (vp_pio_dev) from (3, pio, 0x3f8, &ops)) return 1;

#ifdef PRESET_LINUX
#include <sys/mman.h>
    u8_t *ua = mmap               (
        0                         ,
        4 KB                      ,
        PROT_READ | PROT_WRITE    ,
        MAP_ANONYMOUS | MAP_SHARED,
        -1                        ,
        0
    );
#endif

    if (!make (vp_map) from (3, pa, ua, 4 KB)) return 1;
    mem_copy (ua, code, sizeof(code));

    vp_reg reg;
    vp_cpu_sync_out(cpu, &reg);

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

    reg.gen.rflags = 2ull;
    reg.gen.rip    = 1 MB;
    reg.gen.rax    = 0x03;
    reg.gen.rbx    = 0x04;

    vp_cpu_sync_in (cpu, &reg);

    u64_t ret = (u64_t) await(vp_cpu_run(cpu));
    if (ret == -1) printf ("CPU ERR CODE   : %d\n", vp_cpu_error(cpu));
    else           printf ("CPU RUN RESULT : %d\n", ret);
    del (root);
    return 0;
}