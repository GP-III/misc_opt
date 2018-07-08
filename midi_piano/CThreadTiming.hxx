//  CThreadTiming.hxx 
//  Timing for thread 
//  
//  about QueryPerformanceCounter, 
//  see MSDN's Game Timing and Multicore Processors
//  https://msdn.microsoft.com/en-us/library/ee417693(v=vs.85).aspx

# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CTHREADTIMING_INCLUED_HEADER_CXX_)
#  define _CTHREADTIMING_INCLUED_HEADER_CXX_

# include "stdwtl.h"

// TODO: __asm RDTSC ?.. 
class CThreadTiming {
private:
  ULONG64 m_QueryFreq;
  ULONG64 m_Time64Check;

  BOOL m_bUseMMSysmtem;
  HANDLE m_hWaitTiming; // emmm....

  DWORD m_dwTimingStart;
  DWORD m_dwTimingUpdate;
  // void BindThreadCore () 
public:
  struct TSTiming {
    DWORD cur;
    DWORD elapse;
  };

  DWORD GetCurrentTimeCache (void) {
    return m_dwTimingUpdate;
  }
  CThreadTiming (void)  {
    m_dwTimingStart = 0;
    m_Time64Check = 0;
    m_dwTimingUpdate = 0;
    m_bUseMMSysmtem = TRUE;
    m_hWaitTiming = CreateEvent (NULL, NULL, NULL, NULL);
    ATLASSERT (m_hWaitTiming != NULL);
    BOOL sig = QueryPerformanceFrequency ((LARGE_INTEGER *)& m_QueryFreq);
    ATLASSERT (sig != FALSE);
  }
~ CThreadTiming (void)  {
    CloseHandle (m_hWaitTiming);
    m_hWaitTiming = NULL;
  }
 
  BOOL SetThreadAffinity (int cpu_mask) {
    return (
       0 == SetThreadAffinityMask (GetCurrentThread (), (DWORD_PTR)cpu_mask)
       ?  FALSE
       : TRUE);
  }

  BOOL SetThreadPri (int pri) {
    return SetThreadPriority (GetCurrentThread (), pri);
  }

  BOOL SetThreadBoost (BOOL bFixedPri = FALSE) {
    return SetThreadPriorityBoost (GetCurrentThread (), bFixedPri);
  }

  BOOL isMMSystemTiming(void) {
    return !! m_bUseMMSysmtem;
  }
  void GetTimingStart(DWORD *ms) {
    if (ms != NULL)
      *ms = m_dwTimingStart;
  }

  void UseMMSystemTiming (BOOL bMMSystemTiming) {
    m_bUseMMSysmtem = !! bMMSystemTiming;
  }

  void SetTimingStart (DWORD *cur)  {
    if (m_bUseMMSysmtem != FALSE)  {
      m_dwTimingStart = timeGetTime ();   
    } else   {
      ULONG64 ut64;
      QueryPerformanceCounter ((LARGE_INTEGER *)& ut64);
      m_dwTimingStart = (DWORD) (ut64*1000/ m_QueryFreq);
      m_Time64Check = ut64;
    }
    if (cur != NULL)
      *cur = m_dwTimingStart;
  }

  void GetTimingCurrent (DWORD *ms, DWORD *elapse)  {
    DWORD t;
    if (m_bUseMMSysmtem != FALSE)  {
      t = timeGetTime ();
    } else   {
      ULONG64 ut64;
      QueryPerformanceCounter ((LARGE_INTEGER *)& ut64);
      if  ( ( (LONG64)ut64) < ( (LONG64)m_Time64Check))  {
        // Reset in system sleep.>.
        //  m_Time64Check = ut64;
        m_dwTimingStart = (DWORD) (ut64*1000/ m_QueryFreq);
      }
      t = (DWORD) (ut64*1000/ m_QueryFreq);
      m_Time64Check = ut64;
    }
    m_dwTimingUpdate = t;
    if (ms != NULL)  
      *ms = t;
    if (elapse != NULL) 
      *elapse = t - m_dwTimingStart;
  }
  DWORD GetTimingCurrent (void)  {
    DWORD t;
    if (m_bUseMMSysmtem != FALSE)  {
      return timeGetTime ();
    } else   {
      ULONG64 ut64;
      QueryPerformanceCounter ((LARGE_INTEGER *)& ut64);
      return (DWORD) (ut64*1000/ m_QueryFreq);
    }
  }  
  void GetTimingCurrent (TSTiming &t)  {
    GetTimingCurrent (& t.cur, & t.elapse);
  }
  //  delay time, thread spin or nt kernel sleep
  void DelayMS (DWORD ms, BOOL bThreadSpin = FALSE) {

    if (bThreadSpin == FALSE) {
      DWORD sig_0 = WaitForSingleObject (m_hWaitTiming, ms);
      ATLASSERT (sig_0 == WAIT_TIMEOUT);
    } else if (m_bUseMMSysmtem != FALSE)  {
      DWORD t = timeGetTime ();
      while ( ( timeGetTime () - t) > ms) 
        continue  ;
    } else   {
      ULONG64 ut64;
      QueryPerformanceCounter ((LARGE_INTEGER *)& ut64);
      ULONG64 ticks = m_QueryFreq / 1000 * (ULONG64) ms;
      while  (TRUE   )  {
        ULONG64 ut64_cur;
        QueryPerformanceCounter ((LARGE_INTEGER *)& ut64_cur);
        if  ( (ut64_cur - ut64) > ticks) 
           break  ;
      }
    }
  }
};

# endif 