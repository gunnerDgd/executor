#include <core.h>
#include <thread.h>
#include <io.h>

#include <vp/core.h>

#include <stdio.h>

use (dep(vp_core), dep (io), dep (thread))

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
    vp_pa   *pa   = make (vp_pa)   from (3, root, 4 KB, 4 KB);
    vp_cpu  *cpu  = make (vp_cpu)  from (2, root, 0);

#ifdef PRESET_LINUX
#include <sys/mman.h>
    u8_t    *ua   = mmap          (
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

    reg.cs.base     = 0ull;
    reg.cs.selector = 0ull;

    reg.gen.rflags = 2ull;
    reg.gen.rip    = 4 KB;
    reg.gen.rax    = 0x03;
    reg.gen.rbx    = 0x04;

    vp_cpu_sync_in (cpu, &reg);

    fut  *fut = vp_cpu_run(cpu);
    u64_t ret = (u64_t) await(fut);

    if (ret != -1) printf ("CPU RUN RESULT : %d\n", ret);
    else           printf ("ERR : %d\n", vp_err(cpu));
    return 0;
}