#include <Windows.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "futex.h"

__declspec(thread) void *localaddress;


struct futex_ {

  int sig; //     MSB:锁定信号 OTHER:当前一同竞争的线程, XXX:ILP64
  void *own;       //  指示当前拥有锁的ID
  int recur;           //  递归计数
  HANDLE _destroy, _lock; // manual-reset/ auto-reset event. 
};


static 
int atomic_inc_Z80000000h ( volatile int *addend) {
  
  {
    int previous;
    int current; 
    
    do 
    { 
      if ( (previous = addend[0]) == 0) current = 0x80000000;     
      else                        current = previous + 1;            
      
    } while ( 
    InterlockedCompareExchange 
    (  addend, current, previous) != previous); return previous;
  }
}


es_status 
futex_init (futex_t *fu) {

  struct futex_ *mtxp = (struct futex_ *) malloc (sizeof (*mtxp));
  assert (mtxp != NULL);

  mtxp->sig     = 0;
  mtxp->own            = NULL;
  mtxp->_lock     = CreateEvent (NULL, FALSE, FALSE, NULL);
  mtxp->_destroy  = CreateEvent (NULL, TRUE, FALSE, NULL);
  mtxp->recur  =0;

  assert (mtxp->_lock != NULL);
  assert (mtxp->_destroy != NULL);

  *fu = (futex_t) mtxp;
  return ES_OK;
}

es_status
futex_lock (futex_t fu) {

  __try {

    struct futex_ *mtxp = (struct futex_ *) fu;
    
    //当前拥有者??? 
    if (mtxp->own == & localaddress) 
         {
            mtxp->recur++;
            return ES_OK;
         }

    if ( atomic_inc_Z80000000h (& mtxp->sig) != 0)
         {
             DWORD waitResult;
             HANDLE handle_g[2];
             
            /*
             *映射多句柄
             */
             handle_g[0] = mtxp->_lock;
             handle_g[1] = mtxp->_destroy;        

            /* 给系统调度器
             */
             waitResult = WaitForMultipleObjects
                         ( 2, &handle_g[0], FALSE, INFINITE);         
                         
             switch ( waitResult )
                   {
                   case WAIT_OBJECT_0:
                    /*
                     * 取消一个等待
                     */ 
                     InterlockedExchangeAdd (& mtxp->recur, -1);
                     break;
                     
                   case WAIT_OBJECT_0 + 1: 
                     return ES_NOEXIST;  // 当前守护的对象已经被析构
                     
                   case WAIT_FAILED: 
                   default:          
                     return ES_FAIL;   //  乱七八糟的错误                  
                   }
             
         }
         
        /*
         * 第一次获取锁
         */
         mtxp->recur = 1;
         mtxp->own = & localaddress;
         return ES_OK;
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER \
                              : EXCEPTION_CONTINUE_SEARCH) {
    return ES_BADMEM;
  }
}


int futex_unlock (futex_t fu) {

  __try {

    struct futex_ *mtxp = (struct futex_ *) fu;

    /* check owner*/
    if (mtxp->own != & localaddress) {
      return  ES_NOTSELF;
    }
    
    /*
     * FIXME:optimizer will retain the semantics of the operator precedence??
     */
    if ( (--mtxp->recur == 0) ) {
      mtxp->own = NULL; 
      if (InterlockedCompareExchange (& mtxp->sig, 0, 0x80000000) != 0x80000000) {
              if ( SetEvent (mtxp->_lock) == FALSE)
                return ES_FAIL;
            }
    }
    return ES_OK;
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER \
                              : EXCEPTION_CONTINUE_SEARCH) {
    return ES_BADMEM;
  }
}


int futex_destroy (futex_t fu) {

  __try {

    struct futex_ *mtxp = (struct futex_ *) fu;
    HANDLE _lock, _destroy;

    _destroy = mtxp->_destroy;
    _lock = mtxp->_lock;
    
    /*
     * Close _lock handle.
     * Broadcast destroy event .
     */
    CloseHandle (_lock);
    SetEvent (_destroy);
    CloseHandle (_destroy);
    
    /*
     * Destroy mutex object's heap.
     */   
    free (mtxp);
    return ES_OK;
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER \
                              : EXCEPTION_CONTINUE_SEARCH) {
    return ES_BADMEM;
  }
}
