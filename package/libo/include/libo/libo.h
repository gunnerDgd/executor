#ifndef LIBO_LIBO_H
#define LIBO_LIBO_H

#include <core.h>

extern obj_trait *libo_t;
typedef struct    libo { u8_t libo[512]; } libo;

#define libo_loader_unknown   0xff
#define libo_loader_lilo      0x01
#define libo_loader_syslinux  0x03
#define libo_loader_grub      0x07
#define libo_loader_uboot     0x08
#define libo_loader_qemu      0x0B

void   libo_ima_pos   (libo*, u64_t);
void   libo_ima_len   (libo*, u64_t);

void   libo_video     (libo*, u64_t);
void   libo_loader_ver(libo*, u64_t);
void   libo_loader    (libo*, u64_t);

void   libo_command   (libo*, const char*, u64_t);
void   libo_mem       (libo*, vp_map*)           ;

#endif
