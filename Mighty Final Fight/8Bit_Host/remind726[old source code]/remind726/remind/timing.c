#include "system.h"

struct timing_s {
  unsigned int eveticks;
  unsigned int micro;
  unsigned int quad;
  unsigned int period;  
  struct system_s *sys;
#ifdef __WINDOWS_
  int (__stdcall_ *native_timing)(void *stamp, void *native_freq);
  LARGE_INTEGER stat, calc, freq;
#else 
#endif 
};
// FIXED:...
int init_timing (struct system_s *sys) {
#ifdef __WINDOWS_
  struct timing_s *tm = dlmalloc (sizeof(struct timing_s));
  if (!tm)
      return -1;  
  
  tm->native_timing = (void *)GetProcAddress (GetModuleHandleA ("ntdll.dll"), "NtQueryPerformanceCounter");
  if (tm->native_timing == __null_)
      goto __final;
  if (tm->native_timing (&tm->stat, &tm->freq))
      goto __final;

  if (!SetThreadAffinityMask (GetCurrentThread(), 1)) 
      goto __final; 
  if (!SetProcessAffinityMask (GetCurrentProcess(), 1))
      goto __final;
  
  /* Set parent, child */
  tm->sys = sys;
  
  sys->timing_handle = tm;
  return 0;
  
__final:
  /* release timingdev mem */
  dlfree(tm);

  return -1;
#else 
#endif   
}
int set_timing_period (struct timing_s *tm, int period) {
  tm->quad = 0;
  tm->micro = 0;
  tm->period = period;
#ifdef __WINDOWS_  
  tm->eveticks = tm->freq.QuadPart / (LONGLONG)period;
#else 
#endif 
  return 0;
}
int start_timing (struct timing_s *tm) {
#ifdef __WINDOWS_   
  if (tm->native_timing (&tm->stat, &tm->freq))
      return -1;
  return 0;
#else 
#endif
}
int terming_timing (struct timing_s *tm) {
#ifdef __WINDOWS_   
  if (tm->native_timing (&tm->calc, &tm->freq))
      return -1;
  
  while (tm->calc.QuadPart - tm->stat.QuadPart < tm->eveticks)
         if (tm->native_timing (&tm->calc, &tm->freq))
             return -1;
  tm->micro += 1;
  if (tm->micro >= tm->period)
      tm->quad += tm->micro/tm->period, tm->micro %= tm->period;
  return 0;
#else 

#endif    
}
unsigned int getsmall_timing (struct timing_s *tm) {  
  return tm->micro;  
}
unsigned int getquad_timing (struct timing_s *tm) {  
  return tm->quad; 
}
void uninit_timing (struct timing_s *tm) {
#ifdef __WINDOWS_   
  if (tm)
      dlfree(tm);
#else 
#endif
}