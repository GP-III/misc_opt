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

#include "gameboy.h"
#include "internal.h"

void controller_setupdate_ (struct controller *ctl, void (*update) 
            (struct controller *, 
            void *, 
              struct controller_pad *, /* self */ 
            struct controller_pad * /* host edge */), void *obj);
void ppu_setupdate_ (struct ppu *ppu, void (*update) 
            (struct ppu *, 
            void *, 
              struct ppu_framebuffer *), void *obj);

static 
void controller_hostdrv_s (struct gameboy *gb, void *controller_drvobj,
                         struct controller_pad *gb_pad, /* gb-self for recv joypadbuffer */ 
                         struct controller_pad *hostedge /* host-edge 1?pulse gen:nodone */)
{
  printf ("%s:%s please set controller_hostdrv\n", __FILE__, __LINE__);
  assert (0);
}

static 
void display_hostdrv_s (struct gameboy *gb, 
                      void *display_drvobj,
                      struct ppu_framebuffer *fmebuf)
{
  printf ("%s:%s please set display_hostdrv\n", __FILE__, __LINE__);
  assert (0);
}

static 
void controller_hostdrv_route (struct controller *ctl, void *obj, struct controller_pad *gb_infos, struct controller_pad *host_infos) {
  ctl->gb->controller_hostdrv (ctl->gb, obj, gb_infos, host_infos);
}
static 
void lcdvideo_hostdrv_route (struct ppu *ppu, void *obj, struct ppu_framebuffer *fbuf) {
  ppu->gb->display_hostdrv (ppu->gb, obj, fbuf);
}

void gameboy_controller_setupdate (struct gameboy *gb, void (*controller_hostdrv) 
             (struct gameboy *, 
             void *, 
                struct controller_pad *, /* gb-self for recv joypadbuffer */ 
             struct controller_pad * /* host-edge */), void *obj)
{
  gb->controller_hostdrv = controller_hostdrv;
  gb->controller_drvobj = obj;

  controller_setupdate_ (gb->joypad, controller_hostdrv_route, obj);
}

void gameboy_lcdvideo_setupdate (struct gameboy *gb, void (*lcdvideo_hostdrv) 
             (struct gameboy *, 
              void *, 
              struct ppu_framebuffer *), void *obj)
{
  gb->display_hostdrv = lcdvideo_hostdrv;
  gb->display_drvobj = obj;

  ppu_setupdate_ (gb->lh5028, lcdvideo_hostdrv_route, obj);
}

extern 
ks_int ks_callc 
cpu_optick (struct cpu *cpu_);

ks_finline
ks_void 
internaldev_clks (struct gameboy *gb, ks_double clks) {
  /* update internal device divider */
  gb->cpu_clks_divider += clks;
  gb->cpu_clks_ppu += clks;
  gb->cpu_clks_timer += clks;
  gb->cpu_clks_joypad += clks;
  gb->cpu_clks_apu += clks;
  gb->cpu_clks_cart += clks;
  gb->cpu_clks_serial += clks;
  /* update device */ 
  gb->divider->clks (gb->divider);
  gb->lh5028->clks (gb->lh5028);
  gb->timer->clks (gb->timer);
  gb->joypad->clks (gb->joypad);
  gb->apu->clks (gb->apu);
  gb->cart->clks (gb->cart);
  gb->serial->clks (gb->serial);
}

void /* gameboy's main function */
gameboy_run_ms (struct gameboy *gb, ks_double exec_ms /*neg disable */) {
  
  ks_int opclks_;
  ks_double opclks;
  ks_double clks_b;
  ks_bool fadjust;
  ks_uint8 imask;
  ks_double cpu_clks;
  ks_double op_ms;
  ks_double oc_ms;

  oc_ms = gb->deflect_ms;
  gb->deflect_ms += exec_ms;
  if (ks_cmp0_double (& gb->deflect_ms) == KS_CMP_ABOVE) {
    op_ms = exec_ms -  oc_ms;
    gb->deflect_ms = 0.0;
    clks_b = op_ms * gb->mach_tools->clk_ms;
    cpu_clks = clks_b;
    /* device done loop */
    while (ks_true)  {
      /* check remain clks */
      if (ks_cmp0_double (& cpu_clks) != KS_CMP_ABOVE)  {
        /* cycles burning out **/
        gb->deflect_ms = cpu_clks / gb->mach_tools->clk_ms;
        break ;
      } else {
        /* remain clks to run, first check dma run. */
        if (ks_cmp0_double (& gb->cpu_clks_dma) == KS_CMP_ABOVE) {
          /* DMA active, exec nop instruction, clks:4 */
          gb->cpu_clks_dma -= 4.0;
          cpu_clks -= 4.0; /* XXX:stride clks for next DMA. */
          internaldev_clks (gb, 4.0);
          continue ;
        } else if (gb->lh5028->hdma_gen != ks_false && !gb->lh5028->hdma_r16) {
          gb->lh5028->hdma_gen = ks_false;
          gb->lh5028->reg55_HDMA5 |= 0x80; /* Set unactive */
        }
        /* check stop, about stop opcode 
           see http://gbdev.gg8.se/wiki/articles/Joypad_Input
               http://www.devrs.com/gb/files/faqs.html#STOP */
        if (gb->lr35902->stop != ks_false) {
          /* XXX: in fact not close APU. */
          cpu_clks -= 4.0;
          gb->cpu_clks_joypad += 4.0;
          gb->joypad->clks (gb->joypad);
          continue;
        }
        /* check halt, about halt opcode 
           see. http://www.devrs.com/gb/files/faqs.html#HALT */
        if (gb->lr35902->halt != ks_false/* && gb->lr35902->IME != 0*/) {  
          cpu_clks -= 4.0;
          internaldev_clks (gb, 4.0);
          fadjust = ks_false;
          if ((gb->reg0F_IF & gb->regFF_IE & 0x1F) != 0) {
            gb->lr35902->halt = ks_false;  /* close halt*/
            if (gb->lr35902->IME == 0) {
              /* halt bug gen, read next byte twice */
              gb->lr35902->_backup = -1;
            }
          }
          goto check_interrupt;
        }
        /* run cpu opcode */
        opclks_ = sizeof (*gb->lr35902);
        opclks_ = cpu_optick (gb->lr35902);
        opclks = (ks_double) opclks_;
        /* check KEY1 */
        fadjust = ks_true;
        if (opclks_ & 0x80000000) 
          opclks_ &= 0x7FFFFFFF;
        else fadjust = ks_false;
        cpu_clks -= opclks_;
        internaldev_clks (gb, opclks_);
  check_interrupt:
        if (gb->lr35902->IME != 0) {
          if ((imask = (gb->reg0F_IF & gb->regFF_IE & 0x1F)) != 0) {    
           /* Interrupt Service Routine According to Z80 datasheets, 
              the following occurs when control is being transferred to an interrupt handler: */        
            ks_uint16 iaddr;
            gb->lr35902->halt = ks_false;  /* close halt*/
            if (imask & IRQ_1) { iaddr = IRQ_1_ADDRESS; imask= IRQ_1; }
            else if (imask & IRQ_2) { iaddr = IRQ_2_ADDRESS; imask= IRQ_2; }
            else if (imask & IRQ_3) { iaddr = IRQ_3_ADDRESS; imask= IRQ_3; }
            else if (imask & IRQ_4) { iaddr = IRQ_4_ADDRESS; imask= IRQ_4; }
            else if (imask & IRQ_5) { iaddr = IRQ_5_ADDRESS; imask= IRQ_5; }
            /* 1. Two wait states are executed
              (2 machine cycles pass while nothing occurs, presumably the CPU is executing NOPs during this time). */
            cpu_clks -= 8.0;
            internaldev_clks (gb, 8.0);
            gb->reg0F_IF &=~imask; /*close interrupr request mask */
            gb->lr35902->IME = 0; /* reset IME. */
            gameboy_mmu_write (gb, gb->lr35902->SP - 1, gb->lr35902->PH);
            gameboy_mmu_write (gb, gb->lr35902->SP - 2, gb->lr35902->PL);
            gb->lr35902->SP -= 2;
            /* 2. The current PC is pushed onto the stack, this process consumes 2 more machine cycles. */
            cpu_clks -= 8.0;
            internaldev_clks (gb, 8.0);
            /* 3. The high byte of the PC is set to 0, 
                  the low byte is set to the address of the handler ($40,$48,$50,$58,$60). 
                  This consumes one last machine cycle. */
            gb->lr35902->PC = iaddr;
            cpu_clks -= 4.0;
            internaldev_clks (gb, 4.0);
          }
        }
        if (fadjust != ks_false) {
          /* freq adjust in all device  */
          if (gb->lr35902->reg4D_key1 & 0x80) {
            /* normal to double freq */
            cpu_clks *= std_machine.cgb_gbp_p;
            gb->cpu_clks_ppu *= std_machine.cgb_gbp_p;
            gb->lh5028->hbl_clks_st *= std_machine.cgb_gbp_p;
            gb->lh5028->oambg_clks_divider21 *= std_machine.cgb_gbp_p;
            gb->lh5028->oambg_clks_total *= std_machine.cgb_gbp_p;
            gb->mach_tools = (struct machine_setup *)& adv_machine;
          } else {
            /* double freq to normal */
            cpu_clks *= adv_machine.gbp_cgb_p;
            gb->cpu_clks_ppu *= adv_machine.gbp_cgb_p;
            gb->lh5028->hbl_clks_st *= adv_machine.gbp_cgb_p;
            gb->lh5028->oambg_clks_divider21 *= adv_machine.gbp_cgb_p;
            gb->lh5028->oambg_clks_total *= adv_machine.gbp_cgb_p;
            gb->mach_tools = (struct machine_setup *)& std_machine;
          }
        }
      }
    }
  }
}

int apu_init (struct apu **apu);
int ppu_init (struct ppu **ppu);
int cpu_init (struct cpu **cpu);
int controller_init (struct controller **controller);
int timer_init (struct timer **timer);
int divider_init (struct divider **divider_);
int cartridge_init (struct cartridge ** cartridge);
int serial_init (struct serial ** serial);

int gameboy_init (struct gameboy **gb) {

  struct gameboy *gb_ =ks_null;
  assert (gb != ks_null);

  gb_ = (struct gameboy *)
     calloc (sizeof (struct gameboy), 1);
  apu_init (& gb_->apu);
  ppu_init (& gb_->lh5028);
  cpu_init (& gb_->lr35902);
  controller_init (& gb_->joypad);
  timer_init (& gb_->timer);
  divider_init (& gb_->divider);
  cartridge_init (& gb_->cart);
  serial_init (& gb_->serial);

  /* lnk child objects */
  gb_->apu->gb = gb_;
  gb_->lh5028->gb = gb_;
  gb_->lr35902->gb = gb_;
  gb_->joypad->gb = gb_;
  gb_->timer->gb = gb_;
  gb_->divider->gb = gb_;
  gb_->cart->gb = gb_;
  gb_->serial->gb = gb_;

  gb_->controller_hostdrv = controller_hostdrv_s;
  gb_->display_hostdrv = display_hostdrv_s;

  ppu_setupdate_ (gb_->lh5028, lcdvideo_hostdrv_route, ks_null);
  controller_setupdate_ (gb_->joypad, controller_hostdrv_route, ks_null);

  gb_->reg70_SVBK = 1;
  * gb = gb_;
  return 0;
}

void divider_write (struct divider *divider, ks_uint8 value);
void timer_write (struct timer *timer, ks_uint16 addr, ks_uint8 value);
void ppu_write (struct ppu *ppu, ks_uint16 addr, ks_uint8 value);
void apu_write (struct apu *apu, ks_uint16 addr, ks_uint8 value);
void serial_write (struct serial *apu, ks_uint16 addr, ks_uint8 value);
void cartridge_write (struct cartridge *cart, ks_uint16 addr, ks_uint8 value);
void controller_write (struct controller *cart, ks_uint8 value);

void 
ks_callstd gameboy_mmu_write (struct gameboy *gb, ks_uint16 address, ks_uint8 value) {

  switch (address >> 13) {
  case 0x00: /* Memmap- 0x0000-0x1FFF*/
  case 0x01: /* Memmap- 0x2000-0x3FFF*/
  case 0x02: /* Memmap- 0x4000-0x5FFF*/
  case 0x03: /* Memmap- 0x6000-0x7FFF*/
    gb->cart->write (gb->cart, address, value); /* Program ROM BANK0 or BANK1-NN */
    break;
  case 0x04: /* Memmap- 0x8000-0x9FFF*/
    gb->lh5028->ram[address-0x8000+(gb->lh5028->reg4F_VBK&1)*0x2000] = value;
    break;
  case 0x05: /* Memmap- 0xA000-0xBFFF*/
    gb->cart->write  (gb->cart, address, value); /* SRAM or ext */
    break;
  case 0x06: /* Memmap- 0xC000-0xDFFF*/
    if (address <= 0xCFFF)/* WRAM BANK 0*/
      gb->wram[address-0xC000] = value;
    else /* WRAM BANK 1-7 */
      gb->wram[address-0xD000+(gb->reg70_SVBK & 7)*0x1000] = value;
    break;
  case 0x07: /* Memmap- 0xE000-0xFFFF*/
    if (address <= 0xFDFF) /* echo ram. same as nes's mirror ram. */
      if (address <= 0xEFFF) 
        gb->wram[address-0xE000] = value;
      else 
        gb->wram[address-0xF000+(gb->reg70_SVBK & 7)*0x1000] = value;
    else if (address <= 0xFE9F) 
      ((ks_int8 *)&gb->lh5028->sp[0])[address-0xFE00] = value; 
    else if (address <= 0xFEFF)
      { /* Not Usable */ }
    else if (address <= 0xFF7F) /* device IO control  TODO:Sound Device IO */ 
      switch (address) {
      case 0xFF00: /* P1 */
        controller_write (gb->joypad, value); 
        break;
      case 0xFF01: /* SB */
      case 0xFF02: /* SC */
        serial_write (gb->serial, address, value);
        break;
      case 0xFF04: /* DIV */
        divider_write (gb->divider, value); 
        break;
      case 0xFF05: /* TIMA*/
      case 0xFF06: /* TMA */
      case 0xFF07: /* TAC */
        timer_write (gb->timer, address, value);
        break;
      case 0xFF0F: /* IF */
        gb->reg0F_IF = value;  
        break;
      case 0xFF10: /* NR10 */
      case 0xFF11: /* NR11 */
      case 0xFF12: /* NR12 */
      case 0xFF13: /* NR13 */
      case 0xFF14: /* NR14 */
      case 0xFF16: /* NR21 */
      case 0xFF17: /* NR22 */
      case 0xFF18: /* NR23 */
      case 0xFF19: /* NR24 */
      case 0xFF1A: /* NR30 */
      case 0xFF1B: /* NR31 */
      case 0xFF1C: /* NR32 */
      case 0xFF1D: /* NR33 */
      case 0xFF1E: /* NR34 */
      case 0xFF20: /* NR41 */
      case 0xFF21: /* NR42 */
      case 0xFF22: /* NR43 */
      case 0xFF23: /* NR44 */
      case 0xFF24: /* NR50 */
      case 0xFF25: /* NR51 */
      case 0xFF26: /* NR52 */
      case 0xFF30: /* AUD3WAVERAM */
      case 0xFF31: /* AUD3WAVERAM */
      case 0xFF32: /* AUD3WAVERAM */
      case 0xFF33: /* AUD3WAVERAM */
      case 0xFF34: /* AUD3WAVERAM */
      case 0xFF35: /* AUD3WAVERAM */
      case 0xFF36: /* AUD3WAVERAM */
      case 0xFF37: /* AUD3WAVERAM */
      case 0xFF38: /* AUD3WAVERAM */
      case 0xFF39: /* AUD3WAVERAM */
      case 0xFF3A: /* AUD3WAVERAM */
      case 0xFF3B: /* AUD3WAVERAM */
      case 0xFF3C: /* AUD3WAVERAM */
      case 0xFF3D: /* AUD3WAVERAM */
      case 0xFF3E: /* AUD3WAVERAM */
      case 0xFF3F: /* AUD3WAVERAM */
        apu_write (gb->apu, address, value);
        break;
      case 0xFF40: /* LCDC */
      case 0xFF41: /* LCDS */
      case 0xFF42: /* SCY */
      case 0xFF43: /* SCX */
      case 0xFF44: /* LY */
      case 0xFF45: /* LYC */
      case 0xFF46: /* OAMDMA */
      case 0xFF47: /* BGP -DMG Only */
      case 0xFF48: /* OBP0 -DMG Only */
      case 0xFF49: /* OBP1 -DMG Only */
      case 0xFF4A: /* WY */
      case 0xFF4B: /* WX */
      case 0xFF4F: /* VBK -CBG Only*/
      case 0xFF51: /* HDMA1 -CBG Only*/
      case 0xFF52: /* HDMA2 -CBG Only*/
      case 0xFF53: /* HDMA3 -CBG Only*/
      case 0xFF54: /* HDMA4 -CBG Only*/
      case 0xFF55: /* HDMA5 -CBG Only*/
      case 0xFF68: /* BCPS -CBG Only*/
      case 0xFF69: /* BCPD -CBG Only*/
      case 0xFF6A: /* OCPS -CBG Only*/
      case 0xFF6B: /* OCPD -CBG Only*/
        ppu_write (gb->lh5028, address, value);
        break;
      case 0xFF56: /* RP -CBG Only*/
        break;
      case 0xFF70: /* SVBK -CBG Only*/
        gb->reg70_SVBK = value & 7;
        break;
      case 0xFF4D: /* KEY1 -CBG Only*/
        gb->lr35902->reg4D_key1 = value;
      default:
        break;
      }
    else if (address <= 0xFFFE) /* High RAM */
      gb->hram[address-0xFF80] = value;
    else if (address == 0xFFFF) /* Interrupt Enable */
      gb->regFF_IE = value;
    break;
  default:
    /* never reach here */
    assert (0); 
  }
}

ks_void
ks_callstd gameboy_mmu_write_w (struct gameboy *gb, ks_uint16 address, ks_uint16 value) {
  gameboy_mmu_write (gb, address, value & 0xFF);
  gameboy_mmu_write (gb, address+1, value>> 8);
}

ks_uint8 divider_read (struct divider *divider);
ks_uint8 timer_read (struct timer *timer, ks_uint16 addr);
ks_uint8 ppu_read (struct ppu *ppu, ks_uint16 addr);
ks_uint8 apu_read (struct apu *apu, ks_uint16 addr);
ks_uint8 serial_read (struct serial *apu, ks_uint16 addr);
ks_uint8 cartridge_read (struct cartridge *cart, ks_uint16 addr);
ks_uint8 controller_read (struct controller *controller);

ks_uint8 
ks_callstd gameboy_mmu_read (struct gameboy *gb, ks_uint16 address) {

  switch (address >> 13) {
  case 0x00: /* Memmap- 0x0000-0x1FFF*/
  case 0x01: /* Memmap- 0x2000-0x3FFF*/
  case 0x02: /* Memmap- 0x4000-0x5FFF*/
  case 0x03: /* Memmap- 0x6000-0x7FFF*/
    return gb->cart->read  (gb->cart, address); /* Program ROM BANK0 or BANK1-NN */
  case 0x04: /* Memmap- 0x8000-0x9FFF*/
    return gb->lh5028->ram[address-0x8000+(gb->lh5028->reg4F_VBK&1)*0x2000];
  case 0x05: /* Memmap- 0xA000-0xBFFF*/
    return gb->cart->read (gb->cart, address); /* SRAM or ext */
  case 0x06: /* Memmap- 0xC000-0xDFFF*/
    if (address <= 0xCFFF)/* WRAM BANK 0*/
      return gb->wram[address-0xC000];
    else /* WRAM BANK 1-7 */
      return gb->wram[address-0xD000+(gb->reg70_SVBK & 7)*0x1000];
  case 0x07: /* Memmap- 0xE000-0xFFFF*/
    if (address <= 0xFDFF) /* echo ram. same as nes's mirror ram. */
      if (address <= 0xEFFF) 
        return gb->wram[address-0xE000];
      else 
        return gb->wram[address-0xF000+(gb->reg70_SVBK & 7)*0x1000];
    else if (address <= 0xFE9F) 
      return ((ks_int8 *)&gb->lh5028->sp[0])[address-0xFE00]; 
    else if (address <= 0xFEFF)
      return 0xFF; /* Not Usable */
    else if (address <= 0xFF7F) /* device IO control  TODO:Sound Device IO */ 
      switch (address) {
      case 0xFF00: /* P1 */
        return controller_read (gb->joypad); 
      case 0xFF01: /* SB */
      case 0xFF02: /* SC */
        return serial_read (gb->serial, address);
      case 0xFF04: /* DIV */
        return divider_read (gb->divider); 
      case 0xFF05: /* TIMA*/
      case 0xFF06: /* TMA */
      case 0xFF07: /* TAC */
        return timer_read (gb->timer, address);
      case 0xFF0F: /* IF */
        return gb->reg0F_IF;   
      case 0xFF10: /* NR10 */
      case 0xFF11: /* NR11 */
      case 0xFF12: /* NR12 */
      case 0xFF13: /* NR13 */
      case 0xFF14: /* NR14 */
      case 0xFF16: /* NR21 */
      case 0xFF17: /* NR22 */
      case 0xFF18: /* NR23 */
      case 0xFF19: /* NR24 */
      case 0xFF1A: /* NR30 */
      case 0xFF1B: /* NR31 */
      case 0xFF1C: /* NR32 */
      case 0xFF1D: /* NR33 */
      case 0xFF1E: /* NR34 */
      case 0xFF20: /* NR41 */
      case 0xFF21: /* NR42 */
      case 0xFF22: /* NR43 */
      case 0xFF23: /* NR44 */
      case 0xFF24: /* NR50 */
      case 0xFF25: /* NR51 */
      case 0xFF26: /* NR52 */
      case 0xFF30: /* AUD3WAVERAM */
      case 0xFF31: /* AUD3WAVERAM */
      case 0xFF32: /* AUD3WAVERAM */
      case 0xFF33: /* AUD3WAVERAM */
      case 0xFF34: /* AUD3WAVERAM */
      case 0xFF35: /* AUD3WAVERAM */
      case 0xFF36: /* AUD3WAVERAM */
      case 0xFF37: /* AUD3WAVERAM */
      case 0xFF38: /* AUD3WAVERAM */
      case 0xFF39: /* AUD3WAVERAM */
      case 0xFF3A: /* AUD3WAVERAM */
      case 0xFF3B: /* AUD3WAVERAM */
      case 0xFF3C: /* AUD3WAVERAM */
      case 0xFF3D: /* AUD3WAVERAM */
      case 0xFF3E: /* AUD3WAVERAM */
      case 0xFF3F: /* AUD3WAVERAM */
        return apu_read (gb->apu, address);
      case 0xFF40: /* LCDC */
      case 0xFF41: /* LCDS */
      case 0xFF42: /* SCY */
      case 0xFF43: /* SCX */
      case 0xFF44: /* LY */
      case 0xFF45: /* LYC */
      case 0xFF46: /* OAMDMA */
      case 0xFF47: /* BGP -DMG Only */
      case 0xFF48: /* OBP0 -DMG Only */
      case 0xFF49: /* OBP1 -DMG Only */
      case 0xFF4A: /* WY */
      case 0xFF4B: /* WX */
      case 0xFF4F: /* VBK -CBG Only*/
      case 0xFF51: /* HDMA1 -CBG Only*/
      case 0xFF52: /* HDMA2 -CBG Only*/
      case 0xFF53: /* HDMA3 -CBG Only*/
      case 0xFF54: /* HDMA4 -CBG Only*/
      case 0xFF55: /* HDMA5 -CBG Only*/
      case 0xFF68: /* BCPS -CBG Only*/
      case 0xFF69: /* BCPD -CBG Only*/
      case 0xFF6A: /* OCPS -CBG Only*/
      case 0xFF6B: /* OCPD -CBG Only*/
        return ppu_read (gb->lh5028, address);
      case 0xFF56: /* RP -CBG Only*/
        return 0xFF;
      case 0xFF70: /* SVBK -CBG Only*/
        return (gb->reg70_SVBK & 7);
      case 0xFF4D: /* KEY1 -CBG Only*/
        return gb->lr35902->reg4D_key1;
      default:
        return 0xFF;
      }
    else if (address <= 0xFFFE) /* High RAM */
      return gb->hram[address-0xFF80];
    else if (address == 0xFFFF) /* Interrupt Enable */
      return gb->regFF_IE;
    else 
  default:
    /* never reach here */
    assert (0); 
  }
  return 0xFF;
}

ks_uint16
ks_callstd gameboy_mmu_read_w (struct gameboy *gb, ks_uint16 address) {
  ks_uint16 u = gameboy_mmu_read (gb, address);
  ks_uint16 v = gameboy_mmu_read (gb, address+1) << 8;
  return (u |v);
}

int gameboy_loadrom (struct gameboy *gb, FILE *fp) {
  int cartridge_load (struct cartridge *cart, FILE *cartmem);
  return cartridge_load (gb->cart, fp);
}