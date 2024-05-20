#ifndef VAPOR_CORE_KVM_CPU_IO_H
#define VAPOR_CORE_KVM_CPU_IO_H

#include <core.h>

struct vp_cpu;

void   vp_io_ready(struct vp_cpu*);
u64_t  vp_io_out  (struct vp_cpu*, any_t);
u64_t  vp_io_in   (struct vp_cpu*, any_t);

u64_t  vp_io_port (struct vp_cpu*);
u64_t  vp_io_len  (struct vp_cpu*);
u64_t  vp_io_dir  (struct vp_cpu*);

#endif
