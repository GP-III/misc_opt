#ifndef __FUTEX_INCLUDED_
#define __FUTEX_INCLUDED_

#include  "graphics.h"

#ifdef __cplusplus
extern "C" {
#endif
    
typedef    void *futex_t;

es_status futex_init (futex_t *fu);
es_status futex_lock (futex_t fu);
es_status futex_unlock (futex_t fu);
es_status futex_destroy (futex_t fu);

#ifdef __cplusplus
};
#endif

#endif 




