#ifndef VAPOR_CORE_X86_64_H
#define VAPOR_CORE_X86_64_H

#ifdef PRESET_LINUX
#include "x86_64/kvm.h"
#elif  PRESET_WIN32
#include "x86_64/hyperv.h"
#endif

#endif
