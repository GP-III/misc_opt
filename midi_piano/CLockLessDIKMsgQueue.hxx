# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

//  CLockLessDIKMsgQueue.hxx 
//  Thread-Safe DirectKeyboard Message Queue and a simple thread safe (with spinlock) free list
# if !defined (_CLOCKLESSDIKMSGQueue_INCLUED_HEADER_CXX_)
#  define _CLOCKLESSDIKMSGQueue_INCLUED_HEADER_CXX_

# include "stdwtl.h"

extern "C" {
  //   from  my old project source. 
  int32_t __fastcall store32 (void *p, int32_t val);
  int32_t __fastcall add32 (void *p, int32_t val);
  int64_t __fastcall add64 (void *p, int64_t val);
  void __fastcall or32 (void *p, int32_t val);
  BOOL /*kbool */ __fastcall casdp3 (  void **p, 
                                  void **val_gp, 
                                  void **cmp_gp );
};

class CLockLessDIKMsgQueue {
public:

  static const int DIKMsgBlockNums = 3;

  struct DikRaw {
    BYTE dikMap;
    BOOL bPress;
  };

  struct DikRaw2 {
    BYTE vel; 
    BYTE msg; // MSB ? down : up
  };

  struct DikMsg {
    union {
      struct SigCombo {
        //  D0-D3 count 
        //  D4-D7 
        //     D4 :<- 1?bSlotSet[0] down : up 
        //     D5 :<- 1?bSlotSet[1] down : up
        //     D6 :<- 1?bSlotSet[2] down : up
        //     D7 :<- useless. 
        BYTE bCount;
        BYTE bSlotSet[3]; // Queue 0-2 low to max.
      } SigCombo__;

      LONG dwMixMsg;
    };
    BYTE vel; 
    BYTE msg; // MSB ? down : up
  };

  struct  CLockLessDIKMsgQueueNode {
    union {
      struct  CLockLessDIKMsgQueueNode *level;
      intptr_t int_blk;
    };
    DikMsg dikMsg; 
  };

  struct  CLockLessDIKMsgQueueHeader {
    union {
      struct  CLockLessDIKMsgQueueNode *level;
      intptr_t int_blk;
    };
    intptr_t aba_count;
  };

  // free list .
  struct  CLockLessDIKMsgQueueNodeFreeListNode {
    struct  CLockLessDIKMsgQueueNodeFreeListNode *level;
    struct  CLockLessDIKMsgQueueNode *chunk;
  };

  struct  CLockLessDIKMsgQueueNodeFreeListHeader {
    struct  CLockLessDIKMsgQueueNodeFreeListNode *level;
    int level_count;
  };

  CLockLessDIKMsgQueueHeader m_MsgQueue;
  CLockLessDIKMsgQueueNodeFreeListHeader m_MsgQueueCache;
  CLockLessDIKMsgQueueNodeFreeListHeader m_MsgQueueListBodyCache;
  BOOL m_bMutilLock;

  //  for multithread lock.
  intptr_t m_SpinFreeListLock;

  void SetMultiLockForFressList (BOOL enable) {
    m_bMutilLock = !! enable;
  }

  void push_msg (BYTE sig, BYTE vel) {

    //  push to queue. 
    CLockLessDIKMsgQueueNode *kt = _malloc_node ();
    kt->dikMsg.msg = sig;
    kt->dikMsg.vel = vel;

    //  CAS write it.
    while (TRUE)  {
      CLockLessDIKMsgQueueHeader T, S;

      // load old. 
      T.aba_count = m_MsgQueue.aba_count;
      S.level = T.level = m_MsgQueue.level;
      S.aba_count = T.aba_count + 1;

      // try insert node. 
      kt->level = S.level;
      S.level = kt;

      // TODO: cas128 for return do.
      if ( casdp3 ( (LPVOID *)& m_MsgQueue,
        (LPVOID *)& S,
        (LPVOID *)& T) != FALSE)
        return  ;
    }
  }

  void ClearQueue (void) {
    CLockLessDIKMsgQueueUninit (& m_MsgQueue);
  }

  void push_msg (DikRaw *dik_raw, int count) {
    ATLASSERT (count >=0 && (count <= 3));

    DikMsg t;
    t.dwMixMsg = 0;
    t.SigCombo__.bCount = count;

    for (int id = 0; id != count; id++) {
      t.SigCombo__.bSlotSet[id] = dik_raw[id].dikMap;
      if (dik_raw[id].bPress != FALSE) 
        t.SigCombo__.bCount |= (0x10 << id);
    }

    //  push to queue. 
    CLockLessDIKMsgQueueNode *kt = _malloc_node ();
    kt->dikMsg.dwMixMsg = t.dwMixMsg;
    
    //  CAS write it.
    while (TRUE)  {
      CLockLessDIKMsgQueueHeader T, S;

      // load old. 
      T.aba_count = m_MsgQueue.aba_count;
      S.level = T.level = m_MsgQueue.level;
      S.aba_count = T.aba_count + 1;

      // try insert node. 
      kt->level = S.level;
      S.level = kt;

      // TODO: cas128 for return do.
      if ( casdp3 ( (LPVOID *)& m_MsgQueue,
        (LPVOID *)& S,
        (LPVOID *)& T) != FALSE)
        return  ;
    }
  }

  void push_msg (CDirectInput::DiKeyBufferChunk *dik_buf, int count) {
    ATLASSERT (count >=0 && (count <= 3));

    DikMsg t;
    t.dwMixMsg = 0;
    t.SigCombo__.bCount = count;

    for (int id = 0; id != count; id++) {
      t.SigCombo__.bSlotSet[id] = dik_buf[id].DIK_map;
      if (dik_buf[id].key_sig & 0x80) 
        t.SigCombo__.bCount |= (0x10 << id);
    }

    //  push to queue. 
    CLockLessDIKMsgQueueNode *kt = _malloc_node ();
    kt->dikMsg.dwMixMsg = t.dwMixMsg;
    
    //  CAS write it.
    while (TRUE)  {
      CLockLessDIKMsgQueueHeader T, S;

      // load old. 
      T.aba_count = m_MsgQueue.aba_count;
      S.level = T.level = m_MsgQueue.level;
      S.aba_count = T.aba_count + 1;

      // try insert node. 
      kt->level = S.level;
      S.level = kt;

      // TODO: cas128 for return do.
      if ( casdp3 ( (LPVOID *)& m_MsgQueue,
        (LPVOID *)& S,
        (LPVOID *)& T) != FALSE)
        return  ;
    }
  }
  void pop_msg (DikRaw2 *dik_raw, int *max_count)  {

    ATLASSERT (dik_raw != NULL);
    ATLASSERT (max_count != NULL);

    DikRaw2 dr_cache[20];
    DWORD c = *max_count;
    BOOL bMalloc = FALSE;
    DikRaw2 *pdr = dr_cache;

    if (c > 20)  {
      pdr = new DikRaw2 [c];
      bMalloc = TRUE;
    }

    CLockLessDIKMsgQueueNode *kt = NULL;

    // try pop head.
    while (TRUE)  {
      CLockLessDIKMsgQueueHeader T, S;

      // load old. 
      T.aba_count = m_MsgQueue.aba_count;
      S.level = T.level = m_MsgQueue.level;
      S.aba_count = T.aba_count + 1;

      // try pop node. 
      if (S.level == NULL)  {
        if (c > 20) 
          delete [] pdr;
      * max_count = 0;
        return ;
      } else  {
        kt = S.level;
        S.level = NULL;
      }
      // TODO: cas128 for return do.
      if ( casdp3 ( (LPVOID *)& m_MsgQueue,
        (LPVOID *)& S,
        (LPVOID *)& T) != FALSE)
        break  ;
    }

    int count =0;

    while (kt != NULL) {
      CLockLessDIKMsgQueueNode *pv = kt->level;
      DikMsg T;
      T.msg = kt->dikMsg.msg;
      T.vel = kt->dikMsg.vel;

      _free_node (kt);

      kt = pv;

      if  (count >= c)
        continue  ;
      pdr[count].msg = T.msg;
      pdr[count].vel = T.vel;
      count ++;
    }

    // scan into array 
    int kd = 0;
    for (int id = count - 1; id != -1; id--, kd++)  {
      dik_raw[kd].msg = pdr[id].msg;
      dik_raw[kd].vel = pdr[id].vel;
    }

    if (c > 20) 
      delete [] pdr;
    * max_count = count;
  }

  void pop_msg (DikRaw *dik_raw, int *max_count)  {

    ATLASSERT (dik_raw != NULL);
    ATLASSERT (max_count != NULL);

    DikRaw dr_cache[20];
    DWORD c = *max_count;
    BOOL bMalloc = FALSE;
    DikRaw *pdr = dr_cache;

    if (c > 20)  {
      pdr = new DikRaw [c];
      bMalloc = TRUE;
    }

    CLockLessDIKMsgQueueNode *kt = NULL;

    // try pop head.
    while (TRUE)  {
      CLockLessDIKMsgQueueHeader T, S;

      // load old. 
      T.aba_count = m_MsgQueue.aba_count;
      S.level = T.level = m_MsgQueue.level;
      S.aba_count = T.aba_count + 1;

      // try pop node. 
      if (S.level == NULL)  {
        if (c > 20) 
          delete [] pdr;
      * max_count = 0;
        return ;
      } else  {
        kt = S.level;
        S.level = NULL;
      }
      // TODO: cas128 for return do.
      if ( casdp3 ( (LPVOID *)& m_MsgQueue,
        (LPVOID *)& S,
        (LPVOID *)& T) != FALSE)
        break  ;
    }

    int count =0;

    while (kt != NULL) {
      CLockLessDIKMsgQueueNode *pv = kt->level;
      DikMsg T;
      T.dwMixMsg = kt->dikMsg.dwMixMsg;

      _free_node (kt);

      kt = pv;

      if  (count >= c)
        continue  ;

      // decode message. 
      for (int id = 0; id != (T.SigCombo__.bCount & 0x0F); id++)  {
        if  (count >= c)
          continue  ;
        
        pdr[count].bPress = !! ((T.SigCombo__.bCount & 0xF0) & (0x10 << id));
        pdr[count].dikMap = T.SigCombo__.bSlotSet[id];
        count ++;
      }
    }

    // scan into array 
    int kd = 0;
    for (int id = count - 1; id != -1; id--, kd++)  {
      dik_raw[kd].bPress = pdr[id].bPress;
      dik_raw[kd].dikMap = pdr[id].dikMap;
    }

    if (c > 20) 
      delete [] pdr;
    * max_count = count;
  }

  // do free list stuff. 
  CLockLessDIKMsgQueueNode  *_malloc_node (void)  {
    CLockLessDIKMsgQueueNode *pt = NULL;

    if  (m_bMutilLock != FALSE)  {
      while (InterlockedExchangePointer ((PVOID *)& m_SpinFreeListLock, (PVOID) 1) != NULL)
        _mm_pause (); // __cpu_relaxed.
    }

    // check cache block alive. 
    if (m_MsgQueueCache.level_count > 0) {
      CLockLessDIKMsgQueueNodeFreeListNode *next;
      CLockLessDIKMsgQueueNodeFreeListNode *cur;

      m_MsgQueueCache.level_count --;

      // remove object node.
      next = m_MsgQueueCache.level->level;
      cur = m_MsgQueueCache.level;
      m_MsgQueueCache.level = next; 

      pt = cur->chunk;

      cur->chunk = NULL;

      // list body drop into the dummy list 
      cur->level = m_MsgQueueListBodyCache.level;
      m_MsgQueueListBodyCache.level = cur;
      m_MsgQueueListBodyCache.level_count ++;

    }  else   pt = (CLockLessDIKMsgQueueNode *) malloc (sizeof( CLockLessDIKMsgQueueNode));
    pt->level = NULL;

    //  unlock spinlock.
    if  (m_bMutilLock != FALSE) 
      InterlockedExchangePointer ((PVOID *)& m_SpinFreeListLock, (PVOID) NULL);
    return  pt;
  }

  void _free_node (CLockLessDIKMsgQueueNode *p)  {

    // drop into cache. 
    // check cache body nums. 
    if  (m_bMutilLock != FALSE)  {
      while (InterlockedExchangePointer ((PVOID *)& m_SpinFreeListLock, (PVOID) 1) != NULL)
        _mm_pause (); // __cpu_relaxed.
    }

    CLockLessDIKMsgQueueNodeFreeListNode *C = NULL;
    if ( m_MsgQueueListBodyCache.level_count > 0)  {
      // body list to cache 
      m_MsgQueueListBodyCache.level_count --;

      CLockLessDIKMsgQueueNodeFreeListNode *next;

      // remove object node.
      next = m_MsgQueueListBodyCache.level->level;
      C = m_MsgQueueListBodyCache.level;
      m_MsgQueueListBodyCache.level = next; 
    } else   C = 
        (CLockLessDIKMsgQueueNodeFreeListNode *) malloc (sizeof( CLockLessDIKMsgQueueNodeFreeListNode));;
    C->chunk = p;
    C->level = m_MsgQueueCache.level;
    m_MsgQueueCache.level = C;
    m_MsgQueueCache.level_count ++;

    if  (m_bMutilLock != FALSE) 
      InterlockedExchangePointer ((PVOID *)& m_SpinFreeListLock, (PVOID) NULL);
  }

  void CLockLessDIKMsgQueueNodeFreeListUninit (struct CLockLessDIKMsgQueueNodeFreeListHeader *p) {
    int id = p->level_count;
    int g;
    struct CLockLessDIKMsgQueueNodeFreeListNode *cclink = p->level;
    struct CLockLessDIKMsgQueueNodeFreeListNode *cccache;
    for (g = 0; g != id; cclink = cccache, g++) {
      struct CLockLessDIKMsgQueueNode *uc = cclink->chunk;
      cccache = cclink->level;
      if (uc != NULL) {
        free (uc);
        free (cclink);
      }
    }
  }
  void CLockLessDIKMsgQueueUninit (struct CLockLessDIKMsgQueueHeader *p) {
    struct CLockLessDIKMsgQueueNode *cclink;
    struct CLockLessDIKMsgQueueNode *cccache;
    for (cclink = p->level; cclink != NULL; cclink = cccache) {
      struct CLockLessDIKMsgQueueNode *uc = cclink->level;
      cccache = cclink->level;
      free (cclink);
    }
    p->level = NULL;
  }
~ CLockLessDIKMsgQueue (void) {
    CLockLessDIKMsgQueueNodeFreeListUninit (& m_MsgQueueCache);
    CLockLessDIKMsgQueueNodeFreeListUninit (& m_MsgQueueListBodyCache);
  }
  CLockLessDIKMsgQueue (void) {
    m_MsgQueue.int_blk = 0;
    m_MsgQueue.aba_count = 0;
    m_MsgQueueCache.level = NULL;
    m_MsgQueueCache.level_count = 0;
    m_MsgQueueListBodyCache.level = NULL;
    m_MsgQueueListBodyCache.level_count = 0;

    m_bMutilLock = TRUE;

    m_SpinFreeListLock = 0;

    // prealloc. 
    CLockLessDIKMsgQueueNode *T[64];
    for (int id = 0; id != 64; id++) 
      T[id] = _malloc_node (); 
    for (int id = 0; id != 64; id++) 
      _free_node (T[id] );
  }
};

# endif // !defined(_CLOCKLESSDIKMSGQueue_INCLUED_HEADER_CXX_)