#ifndef VAPOR_CORE_H
#define VAPOR_CORE_H

#include "core/bus.h"
#include "core/cpu.h"
#include "core/irq.h"
#include "core/map.h"
#include "core/pa.h"
#include "core/pic.h"
#include "core/root.h"
#include "core/va.h"

extern obj_trait *vp_core_t;
struct            vp_core { u8_t core[2 KB]; };

extern struct vp_core vp_core;

#endif
