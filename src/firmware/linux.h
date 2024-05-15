#ifndef VAPOR_FIRMWARE_LINUX_H
#define VAPOR_FIRMWARE_LINUX_H

#include "../core.h"
#include <fs.h>

#ifdef PRESET_COMPILER_MSVC
#pragma pack(push, 1)
#endif
typedef struct linux_arg {
    u8_t	setup_sects;
    u16_t	root_flags;
    u32_t	syssize;
    u16_t	ram_size;
    u16_t	vid_mode;
    u16_t	root_dev;
    u16_t	boot_flag;
    u16_t	jump;
    u32_t	header;
    u16_t	version;
    u32_t	realmode_swtch;
    u16_t	start_sys_seg;
    u16_t	kernel_version;
    u8_t	type_of_loader;
    u8_t	loadflags;
    u16_t	setup_move_size;
    u32_t	code32_start;
    u32_t	ramdisk_image;
    u32_t	ramdisk_size;
    u32_t	bootsect_kludge;
    u16_t	heap_end_ptr;
    u8_t	ext_loader_ver;
    u8_t	ext_loader_type;
    u32_t	cmd_line_ptr;
    u32_t	initrd_addr_max;
    u32_t	kernel_alignment;
    u8_t	relocatable_kernel;
    u8_t	min_alignment;
    u16_t	xloadflags;
    u32_t	cmdline_size;
    u32_t	hardware_subarch;
    u64_t	hardware_subarch_data;
    u32_t	payload_offset;
    u32_t	payload_length;
    u64_t	setup_data;
    u64_t	pref_address;
    u32_t	init_size;
    u32_t	handover_offset;
    u32_t	kernel_info_offset;
#ifdef  PRESET_COMPILER_GCC
}  linux_arg __attribute__((packed));
#elif   PRESET_COMPILER_MSVC
#pragma pack(pop)
#endif

extern obj_trait *linux_firm_t;
typedef struct    linux_firm  {
    obj        head;
    mem       *firm,
              *kern,
              *ram;
    linux_arg *arg;
}   linux_firm;

bool_t linux_firm_new    (linux_firm*, u32_t, va_list);
bool_t linux_firm_clone  (linux_firm*, linux_firm*)   ;
void   linux_firm_del    (linux_firm*)                ;

void   linux_par_cstr    (linux_firm*, const char*, u64_t);
void   linux_par         (linux_firm*, str*)              ;

void   linux_loader_flag (linux_firm*, u64_t);
void   linux_loader_ver  (linux_firm*, u64_t);
void   linux_loader      (linux_firm*, u64_t);
void   linux_video       (linux_firm*, u64_t);
void   linux_heap        (linux_firm*, u64_t);

#endif
