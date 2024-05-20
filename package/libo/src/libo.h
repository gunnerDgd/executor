#ifndef LIBO_LIBO_H
#define LIBO_LIBO_H

#include <vp/core.h>
#include <fs.h>

#include "kernel.h"
#include "image.h"
#include "arg.h"

struct boot_e820_entry;
struct boot_params  ;
struct setup_header ;
struct screen_info  ;
struct boot_params  ;
struct apm_bios_info;
struct ist_info     ;
struct efi_info     ;

/*libo Means LInux BOot*/
extern obj_trait *libo_t;
typedef struct    libo          {
    obj                     head;
    vp_root                *root;
    libo_arg                arg;
    libo_ker                ker;
    libo_ima                ima;

    struct screen_info     *video;
    struct setup_header    *conf;
    struct boot_params     *boot;
    struct boot_e820_entry *mem;
    struct apm_bios_info   *apm;
    struct ist_info        *ist;
    struct efi_info        *efi;
}   libo;

bool_t  libo_new       (libo*, u32_t, va_list);
bool_t  libo_clone     (libo*, libo*)         ;
void    libo_del       (libo*)                ;

void    libo_ima_pos   (libo*, u64_t);
void    libo_ima_len   (libo*, u64_t);

void    libo_video     (libo*, u64_t);
void    libo_loader_ver(libo*, u64_t);

#define libo_loader_unknown   0xff
#define libo_loader_lilo      0x01
#define libo_loader_syslinux  0x03
#define libo_loader_grub      0x07
#define libo_loader_uboot     0x08
#define libo_loader_qemu      0x0B
void    libo_loader    (libo*, u64_t);

void    libo_command   (libo*, const char*, u64_t);
void    libo_mem       (libo*, vp_map*)           ;

#endif
