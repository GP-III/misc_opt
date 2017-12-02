#ifndef __timing_
#define __timing_ 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
// XXX:nocheck 
int init_timing (void *sys);
int set_timing_period (void *tm, int period);
unsigned int getsmall_timing (void *tm);
unsigned int getquad_timing (void *tm);
int start_timing (void *tm);
int terming_timing (void *tm);
void uninit_timing (void *tm);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif