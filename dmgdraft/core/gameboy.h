/* nintendo gameboy 
 * https://en.wikipedia.org/wiki/Game_Boy
 * 
 * Copyright (C) 2018 moecmks
 * This file is part of KS3578.
 * 
 * do What The Fuck you want to Public License
 * 
 * Version 1.0, March 2000
 * Copyright (C) 2000 Banlu Kemiyatorn (]d).
 * 136 Nives 7 Jangwattana 14 Laksi Bangkok
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 * 
 * Ok, the purpose of this license is simple
 * and you just
 * 
 * DO WHAT THE FUCK YOU WANT TO.
 */

#ifndef _GAMEBOY_H
#define _GAMEBOY_H 1

#include "conf.h"

#ifdef __cplusplus
extern "C" {
#endif  

struct gameboy;

void gameboy_run_ms (struct gameboy *gb, ks_double ms);
void gameboy_run_frame (struct gameboy *gb);
int gameboy_loadrom (struct gameboy *gb, FILE *fp);
int gameboy_reset (struct gameboy *gb);
int gameboy_kinit (void);
int gameboy_kuninit (void);
int gameboy_init (struct gameboy **gb);
int gameboy_uninit (struct gameboy **gb);
int gameboy_startup (struct gameboy *gb);
int gameboy_suspend (struct gameboy *gb);
int gameboy_issuspend (struct gameboy *gb);

/* host device callback for input */
struct controller_pad {
  ks_uint8 right:1;
  ks_uint8 left:1;
  ks_uint8 up:1;
  ks_uint8 down:1;
  ks_uint8 a:1;
  ks_uint8 b:1;
  ks_uint8 select:1;
  ks_uint8 start:1;
};
void gameboy_controller_setupdate (struct gameboy *gb, void (*controller_hostdrv) 
             (struct gameboy *, 
             void *, 
                struct controller_pad *, /* gb-self for recv joypadbuffer */ 
             struct controller_pad * /* host-edge */), void *obj);

/* host device callback for lcd video */
struct ppu_framebuffer {
  ks_int32 w;
  ks_int32 h;
  ks_uint16 *buf; /* gameboy use AXR5G5B5 pixel format */
  ks_int32 pitch;
};
void gameboy_lcdvideo_setupdate (struct gameboy *gb, void (*lcdvideo_hostdrv) 
             (struct gameboy *, 
              void *, 
              struct ppu_framebuffer *), void *obj);

#ifdef __cplusplus
}
#endif 

#endif 



#  if 0


int gameboy_force (struct gameboy **gb);



/* IRQ Interrupt mask */
static const ks_uint16 IRQ_1 = 0x01;  /*  VBLANK (NMI)  */
static const ks_uint16 IRQ_2 = 0x02;  /*  LCDC  */
static const ks_uint16 IRQ_3 = 0x04;  /*  Programmable timer */
static const ks_uint16 IRQ_4 = 0x08;  /*  Serial port switching */
static const ks_uint16 IRQ_5 = 0x10;  /*  P14-15 Descent edge acknowledge */
static const ks_uint16 IRQ_NIL = 0xFF;  /*  P14-15 Descent edge acknowledge */

/* Interrupt exec address */ 
static const ks_uint16 IRQ_1_ADDRESS = 0x40;
static const ks_uint16 IRQ_2_ADDRESS = 0x48;
static const ks_uint16 IRQ_3_ADDRESS = 0x50;
static const ks_uint16 IRQ_4_ADDRESS = 0x58;
static const ks_uint16 IRQ_5_ADDRESS = 0x60;

/* _interrupt_ctl::rd_wr_mask */
static const ks_uint8 INTERRUPT_CTL_IME_MASK = 1; /* ! only for read */
static const ks_uint8 INTERRUPT_CTL_IF_MASK = 2;
static const ks_uint8 INTERRUPT_CTL_IE_MASK = 4;



struct _interrupt_ctl {
  ks_uint8 IME;
  ks_uint8 IF;
  ks_uint8 IE;
};





int gameboy_init (struct gameboy **gb);
int gameboy_uninit (struct gameboy **gb);
int gameboy_loadrom (struct gameboy *gb, FILE *fp);
int gameboy_interrupt_readf (struct gameboy *gb, struct _interrupt_ctl *int_ctl, ks_uint8 read_flags);
int gameboy_interrupt_writef (struct gameboy *gb, struct _interrupt_ctl *int_ctl, ks_uint8 write_flags);
int gameboy_interrupt_request (struct gameboy *gb, ks_uint16 int_mask); /* set IF flags */
int gameboy_interrupt_set (struct gameboy *gb, ks_uint16 int_mask);
void gameboy_reset (struct gameboy *gb);
int gameboy_start (struct gameboy *gb);
int gameboy_stop (struct gameboy *gb);
int gameboy_run (struct gameboy *gb);

ks_bool gameboy_isoverclocking (struct gameboy *gb);
void gameboy_setoverclocking (struct gameboy *gb);
void gameboy_setstdclocking (struct gameboy *gb);
const struct machine_setup *gameboy_getmachine_setup (struct gameboy *gb);
double gameboy_getcycles (struct gameboy *gb); 
double gameboy_get_ppuclks (struct gameboy *gb, ks_bool *stride_frame); 
void gameboy_setcycles (struct gameboy *gb, double cycles);
ks_uint8 gameboy_mmu_read (struct gameboy *gb, ks_uint16 address);
void gameboy_mmu_write (struct gameboy *gb, ks_uint16 address, ks_uint8 value);
int gameboy_resume_ifstop (struct gameboy *gb);
int  gameboy_hdma_copy (struct gameboy *gb);

# endif 
