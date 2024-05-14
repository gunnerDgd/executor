#ifndef VAPOR_CORE_IRQ_H
#define VAPOR_CORE_IRQ_H

#include <core.h>

#ifdef PRESET_ARCH_X86_64
typedef struct vp_irq_arg {
    u64_t type, // kvm_ioapic_state::delivery_mode    | WHV_INTERRUPT_CONTROL::Type
          mask, // kvm_ioapic_state::mask             | (None)
          mode, // kvm_ioapic_state::dest_mode        | WHV_INTERRUPT_CONTROL::DestinationMode
          dest, // kvm_ioapic_state::dest_id          | WHV_INTERRUPT_CONTROL::Destination
          stat, // kvm_ioapic_state::devlivery_status | (None)
          trig, // kvm_ioapic_state::trig_mode        | WHV_INTERRUPT_CONTROL::TriggerMode
          pol,  // kvm_ioapic_state::polarity         | (None)
          irr,  // kvm_ioapic_state::remote_irr       | (None)
          irq;  // kvm_ioapic_state::vector           | WHV_INTERRUPT_CONTROL::Vector
}   vp_irq_arg;

#define vp_irq_level   1 // vp_irq_arg::trig
#define vp_irq_edge    0 // vp_irq_arg::trig

#define vp_irq_phy     0 // vp_irq_arg::mode
#define vp_irq_log     1 // vp_irq_arg::mode

#define vp_irq_fix     0 // vp_irq_arg:::type
#define vp_irq_pri_low 1 // vp_irq_arg:::type
#define vp_irq_nmi     2 // vp_irq_arg:::type
#define vp_irq_init    3 // vp_irq_arg:::type
#define vp_irq_sipi    4 // vp_irq_arg:::type
#endif

extern obj_trait *vp_irq_t;
typedef struct    vp_irq { u8_t irq[256]; } vp_irq;

bool_t vp_irq_raise(vp_irq*);
void   vp_irq_type (vp_irq*, u64_t);
void   vp_irq_mode (vp_irq*, u64_t);
void   vp_irq_mask (vp_irq*, u64_t);
void   vp_irq_dest (vp_irq*, u64_t);
void   vp_irq_trig (vp_irq*, u64_t);

#endif
