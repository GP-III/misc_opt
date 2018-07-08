# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CMIDIPLAYTHREAD_INCLUED_HEADER_CXX_)
#  define _CMIDIPLAYTHREAD_INCLUED_HEADER_CXX_

# include "stdwtl.h"
# include "resource.h"
# include "CDirectDraw.hxx"
# include "CDirectInput.hxx"
# include "CThreadTiming.hxx"
# include "CLockLessDIKMsgQueue.hxx"

class CPlayerThread  {
public:
  enum EVENT_RECV_TYPE {
    EVENT_RECV_PLAY,
    EVENT_RECV_PLAY_EPS,
    EVENT_RECV_PLAY_BACKGROUND,
    EVENT_RECV_PLAY_FOREGROUND,
    EVENT_RECV_RECORD_START,
    EVENT_RECV_RECORD_END,
    EVENT_RECV_SUSPEND,
    EVENT_RECV_EXIT
  };
  enum EVENT_CURRENT_TYPE {
    EVENT_CUR_USER,
    EVENT_CUR_PLAY_EPS,
    EVENT_CUR_RECORDING
  };
  struct callback_desc {
    void (*cb)(CPlayerThread *, void *param);
    void *param;
  };
  struct callback_desc2 {
    void (*cb)(CPlayerThread *, void *param, BYTE msg, BYTE vel);
    void *param;
  };
  void SetPlayCompelteCallback (void (*cb)(CPlayerThread *, void *), void *param) {
    m_CallBackPlayEpsCompelte.cb = cb;
    m_CallBackPlayEpsCompelte.param = param;
  }

  void SetOctaveAdjustCallback (void (*cb)(CPlayerThread *, void *), void *param) {
    m_CallBackAdjustOctave.cb = cb;
    m_CallBackAdjustOctave.param = param;
  }
  void SetPlayNoteCallback (void (*cb)(CPlayerThread *, void *param, BYTE msg, BYTE vel), void *param) {
    m_CallBackPlayPlayNote.cb = cb;
    m_CallBackPlayPlayNote.param = param;
  }
  // TRUE?vailed key:false
  FORCEINLINE 
  BOOL ProcessKey ( CDirectInput::DiKeyBufferChunk &chunk, CFile_eps::eps_chunk *eps_chunk = NULL)  {
    static int octave_mid[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    static BYTE octave_map[12] = 
       { DIK_F1, DIK_F2, DIK_F3, DIK_F4,
                               DIK_F5, DIK_F6, DIK_F7, DIK_F8,

                               DIK_F9, DIK_F10, DIK_F11, DIK_F12 };
    static int *map_gp[256];
    static bool init_it = false;
    if (init_it == false)  {
      //  lazy init .. .
      ZeroMemory (map_gp, sizeof (map_gp));
      for (int id = 0; id != 12; id++) 
        map_gp[octave_map[id]] = & octave_mid[id];
      init_it = true;
    }
    // check adjust octave. 
    if (map_gp[chunk.DIK_map] != NULL)  {
      if (chunk.key_sig & 0x80)
        m_dwOctaveMiddle = *map_gp[chunk.DIK_map];
      return FALSE;
    } else if (CPianoDraw__::dummt_map[chunk.DIK_map] != NULL)  {
      BYTE sendcmd  = 0x80;
      BYTE mixout;
      if ( chunk.key_sig & 0x80)
        sendcmd = 0x90;
      mixout = m_dwOctaveMiddle * 12;
      mixout += CPianoDraw__::dummt_map[chunk.DIK_map]->midi_pos;
      mixout += m_AttachIoData->OffsetNote;
      mixout &= 0x7F;
      m_AttachMusic->SendMidiMsg ( m_AttachMusic->EncodeMidiMessage (sendcmd, 0, mixout, m_AttachIoData->FixedVel & 0x7F));
      // fill eps chunk.
      if (eps_chunk != NULL) {
        eps_chunk->vel = m_AttachIoData->FixedVel & 0x7F;
        eps_chunk->sig = (sendcmd == 0x90) ? 0x80: 0x00;
        eps_chunk->sig|= mixout;
      }
      return TRUE;
    }
    return FALSE;
  }
  void do_play (BOOL with_record = FALSE)  {

    CDirectInput::DiKeyBuffer buf2;

    if (m_AttachInput->PollBufferFor_BufferMode (& buf2) == 0 && (buf2.vnums > 0))  {
      int id = 0;
      CDirectInput::DiKeyBuffer buf;
      buf.vnums = 0;
      while  (id < buf2.vnums)  {
        if  (ProcessKey (buf2.queue[id]) != FALSE)  {
          buf.queue[buf.vnums].DIK_map = buf2.queue[id].DIK_map;
          buf.queue[buf.vnums].key_sig = buf2.queue[id].key_sig;
          buf.vnums ++; 
        }
        id ++;
      }
      // return  ;
      //  send message for render piano window .
      for  (int id = 0; id != buf.vnums/CLockLessDIKMsgQueue::DIKMsgBlockNums; id++) 
        m_KeyMsgQueue.push_msg (& buf.queue[id*CLockLessDIKMsgQueue::DIKMsgBlockNums], CLockLessDIKMsgQueue::DIKMsgBlockNums);
      if (buf.vnums % CLockLessDIKMsgQueue::DIKMsgBlockNums != 0) 
        m_KeyMsgQueue.push_msg (& buf.queue[
          buf.vnums / CLockLessDIKMsgQueue::DIKMsgBlockNums
                            * CLockLessDIKMsgQueue::DIKMsgBlockNums],
          buf.vnums % CLockLessDIKMsgQueue::DIKMsgBlockNums);
        // set directmusic play note. 
    }
  }
  void do_play_with_record (void)  {

    CDirectInput::DiKeyBuffer buf2;

    if (m_AttachInput->PollBufferFor_BufferMode (& buf2) == 0 && (buf2.vnums > 0))  {
      DWORD current;
      int id = 0;
      CDirectInput::DiKeyBuffer buf;
      CFile_eps::eps_chunk epschunk;
      buf.vnums = 0;
      m_ThreadTiming.GetTimingCurrent (& current, NULL);
      while  (id < buf2.vnums)  {
        if  (ProcessKey (buf2.queue[id], & epschunk) != FALSE)  {
          buf.queue[buf.vnums].DIK_map = buf2.queue[id].DIK_map;
          buf.queue[buf.vnums].key_sig = buf2.queue[id].key_sig;
          buf.vnums ++; 
          m_AttachEps->AppendChunk (epschunk, current);
          if (m_CallBackPlayPlayNote.cb != NULL)
            m_CallBackPlayPlayNote.cb (this, m_CallBackPlayPlayNote.param, epschunk.sig, epschunk.vel);
        }
        id ++;
      }
      // return  ;
      //  send message for render piano window .
      for  (int id = 0; id != buf.vnums/CLockLessDIKMsgQueue::DIKMsgBlockNums; id++) 
        m_KeyMsgQueue.push_msg (& buf.queue[id*CLockLessDIKMsgQueue::DIKMsgBlockNums], CLockLessDIKMsgQueue::DIKMsgBlockNums);
      if (buf.vnums % CLockLessDIKMsgQueue::DIKMsgBlockNums != 0) 
        m_KeyMsgQueue.push_msg (& buf.queue[
          buf.vnums / CLockLessDIKMsgQueue::DIKMsgBlockNums
                            * CLockLessDIKMsgQueue::DIKMsgBlockNums],
          buf.vnums % CLockLessDIKMsgQueue::DIKMsgBlockNums);
        // set directmusic play note. 
    }
  }
  void do_play_eps (void)  {

    BOOL press;
    BYTE note;
    DWORD time;
    BYTE vel;
    BYTE type ;
    BOOL immread ;
    BYTE temp_note;

pp: type = 0x90;
    immread = FALSE;
    int sig = m_AttachEps->ReadChunk (press, note, vel, time);
    if (sig == 0)  {
      if (m_CallBackPlayPlayNote.cb != NULL)
        m_CallBackPlayPlayNote.cb (this, m_CallBackPlayPlayNote.param, note | (press ? 0x80 : 0x00), vel);
    } else return  ;
    if (time != 0) 
      m_ThreadTiming.DelayMS (time, TRUE);
    else 
      immread = TRUE;
    if (press == FALSE)
      type = 0x80;
    m_CurTiming += time;
    temp_note = note;
    note += m_AttachIoData->OffsetNote;
    note &= 0x7F;
    m_AttachMusic->SendMidiMsg ( m_AttachMusic->EncodeMidiMessage (type, 0, note, (/*m_AttachIoData->OffsetNote + */vel) & 0x7F));
    m_KeyMsgQueue.push_msg (temp_note | (press ? 0x80 : 0x00), vel & 0x7F);
    
    if  (immread != FALSE)
      goto pp;
  }
  
public:
  void setEps_attach (CFile_eps *eps) {
    m_AttachEps = eps;
  }
public:
  void SetPlayForeground (void) {
    
    m_recv_event = EVENT_RECV_PLAY_FOREGROUND;
    SetEvent (m_SingalRecvEvent);
    WaitForSingleObject (m_SingalRecvCompeleteEvent, INFINITE);
  }
  void SetPlayEps (void) {
    
    m_recv_event = EVENT_RECV_PLAY_EPS;
    SetEvent (m_SingalRecvEvent);
    WaitForSingleObject (m_SingalRecvCompeleteEvent, INFINITE);
  }
  void SetPlay (void) {
    
    m_recv_event = EVENT_RECV_PLAY;
    SetEvent (m_SingalRecvEvent);
    WaitForSingleObject (m_SingalRecvCompeleteEvent, INFINITE);
  }
  void SetPlayBackground (void) {
    
    m_recv_event = EVENT_RECV_PLAY_BACKGROUND;
    SetEvent (m_SingalRecvEvent);
    WaitForSingleObject (m_SingalRecvCompeleteEvent, INFINITE);
  }
  void SetRecordStart (void) {
    
    m_recv_event = EVENT_RECV_RECORD_START;
    SetEvent (m_SingalRecvEvent);
    WaitForSingleObject (m_SingalRecvCompeleteEvent, INFINITE);
  }
  void SetRecordEnd (void) {
    
    m_recv_event = EVENT_RECV_RECORD_END;
    SetEvent (m_SingalRecvEvent);
    WaitForSingleObject (m_SingalRecvCompeleteEvent, INFINITE);
  }

  void terminate (void) {
    
    m_recv_event = EVENT_RECV_EXIT;
    SetEvent (m_SingalRecvEvent);
    WaitForSingleObject (m_SingalRecvCompeleteEvent, INFINITE);
    WaitForSingleObject (m_ThreadHandle, INFINITE);
  }
  void suspend (void) {
    if (m_HadSuspend != FALSE)
      return  ;  //  single to single nolock. 
    m_recv_event = EVENT_RECV_SUSPEND;
    SetEvent (m_SingalRecvEvent);
    WaitForSingleObject (m_SingalRecvCompeleteEvent, INFINITE);
    m_HadSuspend = TRUE;
  }
  void resume_ (void) {
    if (m_HadSuspend == FALSE)
      return  ;  //  single to single nolock. 
    SetEvent (m_SuspendEvent);
    WaitForSingleObject (m_SingalRecvCompeleteEvent, INFINITE);
    m_HadSuspend = FALSE;
  }

  static unsigned int __stdcall PlayerProc (void *param) {
    CPlayerThread *p = (CPlayerThread *)param;

    while (TRUE)   {

      // Check recv event. 
      DWORD sig = WaitForSingleObject (p->m_SingalRecvEvent, 0);
      ATLASSERT (sig  == WAIT_TIMEOUT || (sig == WAIT_OBJECT_0));

      if (sig == WAIT_TIMEOUT)  {
        // do normal. 
        switch  (p->m_cur_event)  {
        case  EVENT_CUR_USER:
          p->do_play ();
          break  ;
        case  EVENT_CUR_RECORDING:
          p->do_play_with_record ();
          break;
        case  EVENT_CUR_PLAY_EPS:
          p->do_play_eps ();
        default :
          break  ;
        }
      } else  {
        // check event. 
        switch (p->m_recv_event) {
        case  EVENT_RECV_PLAY:
          p->m_KeyMsgQueue.ClearQueue ();
          p->m_cur_event = EVENT_CUR_USER;
          break  ;
        case  EVENT_RECV_PLAY_EPS:
          p->m_KeyMsgQueue.ClearQueue ();
          p->m_CurTiming = 0;
          p->m_AttachEps->ResetPlay (p->m_ThreadTiming.GetTimingCurrent ());
          p->m_cur_event = EVENT_CUR_PLAY_EPS;
          break  ;
        case  EVENT_RECV_PLAY_BACKGROUND:
          p->m_AttachInput->resetPlayType (CDirectInput ::BACKGROUND);
          break  ;
        case  EVENT_RECV_PLAY_FOREGROUND:
          p->m_AttachInput->resetPlayType (CDirectInput ::FOREGROUND);
          break  ;
        case  EVENT_RECV_SUSPEND:
          SetEvent (p->m_SingalRecvCompeleteEvent);
          WaitForSingleObject (p->m_SuspendEvent, INFINITE);
          SetEvent (p->m_SingalRecvCompeleteEvent);
          p->m_AttachMusic->ClearNoise ();
          break;
        case  EVENT_RECV_RECORD_START:
          p->m_KeyMsgQueue.ClearQueue ();
          p->m_cur_event = EVENT_CUR_RECORDING;
          p->m_AttachEps->ResetRecord (p->m_ThreadTiming.GetTimingCurrent ());
          break  ;
        case  EVENT_RECV_RECORD_END:
          p->m_cur_event = EVENT_CUR_USER;
          break  ;
        case  EVENT_RECV_EXIT:
          SetEvent (p->m_SingalRecvCompeleteEvent);
          return 0;
        }
        SetEvent (p->m_SingalRecvCompeleteEvent);
      }  
    }
    return 0;

  }

public:
  void Init (void)  {

    m_dwOctaveMiddle = 7;
    m_CallBackAdjustOctave.cb = NULL;
    m_CallBackPlayEpsCompelte.cb = NULL;
    m_CallBackPlayPlayNote.cb = NULL;

    m_HadSuspend = FALSE;

    m_ThreadTiming.UseMMSystemTiming (FALSE);

    //  Create event..
    m_SuspendEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
    m_SingalSendCompeleteEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
    m_SingalRecvEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
    m_SingalRecvCompeleteEvent = CreateEvent (NULL, FALSE, FALSE, NULL);

    ATLASSERT (m_SuspendEvent != NULL);
    ATLASSERT (m_SingalSendCompeleteEvent != NULL);
    ATLASSERT (m_SingalRecvEvent != NULL);
    ATLASSERT (m_SingalRecvCompeleteEvent != NULL);

    m_cur_event = EVENT_CUR_USER;

    // Create Player thread. 
    m_ThreadHandle = (HANDLE) _beginthreadex (NULL,
              0, PlayerProc, this, 0, 
                 NULL   );

    // Adjust thread priority
    SetThreadAffinityMask (m_ThreadHandle, 2);
    
    SetThreadPriority (m_ThreadHandle, THREAD_PRIORITY_HIGHEST);
    // SetThreadPriority (m_ThreadHandle, THREAD_PRIORITY_TIME_CRITICAL);
    SetThreadPriorityBoost (m_ThreadHandle, TRUE);

    ATLASSERT (m_ThreadHandle != NULL);
  }
public:
  void SetAttach (CDirectInput *input, 
       CDirectMusic2 *music,
       CViewIoInstructmentsBase:: _CViewIoData *iodata,
       CThreadTiming *timing)

  {
    m_AttachInput = input;
    m_AttachMusic = music;
    m_AttachIoData = iodata;
    // m_HostThreadTiming = timing;
  }

public:
  EVENT_RECV_TYPE m_recv_event;
  EVENT_CURRENT_TYPE m_cur_event;

  HANDLE m_SuspendEvent;
  HANDLE m_SingalSendCompeleteEvent;
  HANDLE m_SingalRecvEvent;
  HANDLE m_SingalRecvCompeleteEvent;
  HANDLE m_ThreadHandle;

  CDirectInput *m_AttachInput; // only attach  ! 
  CDirectMusic2 *m_AttachMusic; // only attach !
  CViewIoInstructmentsBase:: _CViewIoData *m_AttachIoData;
  CLockLessDIKMsgQueue m_KeyMsgQueue;
  CThreadTiming m_ThreadTiming;

  CFile_eps *m_AttachEps;

  DWORD m_dwOctaveMiddle;
  DWORD m_CurTiming;

  BOOL m_HadSuspend;

  callback_desc m_CallBackAdjustOctave;
  callback_desc m_CallBackPlayEpsCompelte;
  callback_desc2 m_CallBackPlayPlayNote;
};

# endif // !defined(_CMIDIPLAYTHREAD_INCLUED_HEADER_CXX_)