#ifdef __WINDOWS_
#include "setup.h"
#include <XAudio2.h>

#define INLINE_CTOR($) $(::$)
#define INLINE_CTOR_($) $ = CreateEvent(0,0,0,0)

/* Class-Inline-Rountine_Unwind */
#define STATUS_WAIT_UNWIND($) \
int aSyncWait##$(DWORD waitTime) { \
	return !(WaitForSingleObject(event##$, waitTime) == WAIT_OBJECT_0); \
}
/* <link> Export-Rountine-Unwind */
#define MAPPER_LINK(CToken, Symbol, Type) \
__rttype_ void async_callback_register_##Symbol (class aSyncCallBack *p, void *callback) { \
    p->CToken = (Type)callback; \
}
/* <link> Export-Rountine-Unwind default */
#define MAPPER_LINK_(CToken, callback_suffix) \
__rttype_ void async_callback_set_default_##callback_suffix (class aSyncCallBack *p) { \
    p->CToken = CToken; \
}
/* <wait> Export-Rountine-Unwind */
#define MAPPER_WAIT(CToken, Symbol)\
__rttype_ int async_callback_wait_##Symbol (class aSyncCallBack *p, DWORD waitTime) { \
    return p->aSyncWait##CToken (waitTime); \
}

/*
 * Default callback MSG mapper
 */
static void StreamEnd_          (void){}
static void PassEnd_            (void){}
static void PassStart_          (UINT32 SamplesRequired){}
static void BufferEnd_          (void *BufferContext){}
static void BufferStart_        (void *BufferContext){}
static void LoopEnd_            (void *BufferContext){}
static void EventError_         (void *BufferContext, HRESULT error){}
static void CallBackDestroy_    (void){}

// XAudio2 CallBack Use C++ Interface 
class aSyncCallBack : public IXAudio2VoiceCallback
{
public: 
    // Ctor, set default callback init event 
	aSyncCallBack(void): 
        INLINE_CTOR(StreamEnd_),
        INLINE_CTOR(PassEnd_),
		
        INLINE_CTOR(PassStart_),
        INLINE_CTOR(BufferEnd_),     
        INLINE_CTOR(BufferStart_),
        INLINE_CTOR(LoopEnd_),
        INLINE_CTOR(EventError_),
        INLINE_CTOR(CallBackDestroy_)     
        {
          INLINE_CTOR_(eventStreamEnd_);
          INLINE_CTOR_(eventPassEnd_);
          INLINE_CTOR_(eventPassStart_);
          INLINE_CTOR_(eventBufferEnd_);
          INLINE_CTOR_(eventBufferStart_);
          INLINE_CTOR_(eventLoopEnd_);
          INLINE_CTOR_(eventEventError_); 
        }
    // Dtor
   ~aSyncCallBack(void) {
		CloseHandle (eventBufferStart_);
		CloseHandle (eventBufferEnd_);
		CloseHandle (eventPassStart_);	
		CloseHandle (eventPassEnd_);
		CloseHandle (eventStreamEnd_);
		CloseHandle (eventLoopEnd_);
		CloseHandle (eventEventError_);
        CallBackDestroy_();
    }
    STDMETHOD_(void, OnStreamEnd)                   (void)                                  {SetEvent(eventStreamEnd_);     StreamEnd_();}
	STDMETHOD_(void, OnVoiceProcessingPassEnd)      (void)                                  {SetEvent(eventPassEnd_);       PassEnd_();}
	STDMETHOD_(void, OnVoiceProcessingPassStart)    (UINT32 SamplesRequired)                {SetEvent(eventPassStart_);     PassStart_(SamplesRequired);}
	STDMETHOD_(void, OnBufferEnd)                   (void *BufferContext)                   {SetEvent(eventBufferEnd_);     BufferEnd_(BufferContext);}
	STDMETHOD_(void, OnBufferStart)                 (void *BufferContext)                   {SetEvent(eventBufferStart_);   BufferStart_(BufferContext);}
	STDMETHOD_(void, OnLoopEnd)                     (void *BufferContext)                   {SetEvent(eventLoopEnd_);       LoopEnd_(BufferContext);}
	STDMETHOD_(void, OnVoiceError)                  (void *BufferContext, HRESULT error)    {SetEvent(eventEventError_);    EventError_(BufferContext, error);}
public:
    // Dynamic Callback
	void (*CallBackDestroy_)(void);
    void (*BufferStart_)    (void *);
    void (*BufferEnd_)      (void *);
    void (*PassStart_)      (UINT32);
    void (*PassEnd_)        (void);
    void (*StreamEnd_)      (void);
    void (*LoopEnd_)        (void *);
    void (*EventError_)     (void *, HRESULT);
public:
	HANDLE eventBufferStart_;
	HANDLE eventBufferEnd_;
	HANDLE eventPassStart_;
	HANDLE eventPassEnd_;
	HANDLE eventStreamEnd_;
	HANDLE eventLoopEnd_;
	HANDLE eventEventError_;
public:
    STATUS_WAIT_UNWIND(StreamEnd_);
    STATUS_WAIT_UNWIND(PassEnd_);
    STATUS_WAIT_UNWIND(PassStart_);
    STATUS_WAIT_UNWIND(BufferEnd_);
    STATUS_WAIT_UNWIND(BufferStart_);
    STATUS_WAIT_UNWIND(LoopEnd_);
    STATUS_WAIT_UNWIND(EventError_);
};

extern "C" { 
	void *init_async_callback (void) {
		try {
			return new aSyncCallBack;
		} catch (...) { // cl add command /EHa in release mode
			MessageBoxA (0, "ctor failed", "aSyncCallBack", MB_ICONERROR);
		}
	}
	void uninit_async_callback (class aSyncCallBack *p) {
		try {
			delete p;
		} catch (...) {
			MessageBoxA (0, "dtor failed", "aSyncCallBack", MB_ICONERROR);
		}	
	}
    
	MAPPER_LINK (StreamEnd_,        stream_end,			void (__cdecl_ *)(void));
	MAPPER_LINK (PassEnd_,          pass_end,			void (__cdecl_ *)(void));
	MAPPER_LINK (PassStart_,        pass_start,			void (__cdecl_ *)(UINT32));
	MAPPER_LINK (BufferEnd_,        buffer_end,			void (__cdecl_ *)(void *));
	MAPPER_LINK (BufferStart_,      buffer_start,		void (__cdecl_ *)(void *));
	MAPPER_LINK (LoopEnd_,          loop_end,			void (__cdecl_ *)(void *));
	MAPPER_LINK (EventError_,       event_error,		void (__cdecl_ *)(void *,HRESULT));
	MAPPER_LINK (CallBackDestroy_,  callback_destroy,   void (__cdecl_ *)(void));
    
	MAPPER_LINK_ (StreamEnd_,    stream_end);
	MAPPER_LINK_ (PassEnd_,      pass_end);
	MAPPER_LINK_ (PassStart_,    pass_start);
	MAPPER_LINK_ (BufferEnd_,    buffer_end);
	MAPPER_LINK_ (BufferStart_,  buffer_start);
	MAPPER_LINK_ (LoopEnd_,      loop_end);
	MAPPER_LINK_ (EventError_,   event_error);
    
	MAPPER_WAIT (StreamEnd_,    stream_end);
	MAPPER_WAIT (PassEnd_,      pass_end);
	MAPPER_WAIT (PassStart_,    pass_start);
	MAPPER_WAIT (BufferEnd_,    buffer_end);
	MAPPER_WAIT (BufferStart_,  buffer_start);
	MAPPER_WAIT (LoopEnd_,      loop_end);
	MAPPER_WAIT (EventError_,   event_error);  
}
#else 
#endif 