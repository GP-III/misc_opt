#ifndef __system_
#define __system_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct system_s {
  void *ppu; 
//void *apu; 
  void *video_handle;
//void *sound_handle;
};

void *init_system (void *init_callback, void *mach_callback);
void show_window (void *sys);
void hide_window (void *sys);
void system_swap (void *sys);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif

/* machine_callback (system_s, oamobject, key_infos, timing_quad, timing_small)
 // main_rountine init_args 
 // .... my set 
 // run loop:machine_callback
 // destroy ... 
 // OK  void (*mach_callback)(void *pm, void *sob, void *keymask, unsigned int quad, unsigned int small);
*/

