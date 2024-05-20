#ifndef VAPOR_CORE_KVM_CPU_MMIO_H
#define VAPOR_CORE_KVM_CPU_MMIO_H

#include <core.h>

struct vp_cpu;

void   vp_mmio_ready  (struct vp_cpu*);
u64_t  vp_mmio_out    (struct vp_cpu*, any_t);
u64_t  vp_mmio_in     (struct vp_cpu*, any_t);

u64_t  vp_mmio_addr   (struct vp_cpu*);
u64_t  vp_mmio_len    (struct vp_cpu*);
u64_t  vp_mmio_dir    (struct vp_cpu*);

#endif