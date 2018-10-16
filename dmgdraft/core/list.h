/* ADT-Double List
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

#ifndef _LIST_H
#define _LIST_H 1

#include "conf.h"

struct list_v {
  union {
    void *obj; /*XXX:Sequential dependency, \
                       When necessary, it can be considered as a single linked list and queue */
  };
  union {
    struct list_v *level_atomic; /* for atomic and ABA question */
    struct list_v *level_p;
    int64_t *level_aip64;
    int32_t *level_aip32;
    /* int64_t level_ai64; */
    int32_t level_ai32;
    intptr_t level_aiptr;
  };
  union {
    struct list_v *level_n;/*XXX:Sequential dependency, \
                       When necessary, it can be considered as a single linked list and queue */
    int64_t *level_nip64;
    int32_t *level_nip32;
    /* int64_t level_ni64; */
    int32_t level_ni32;
    intptr_t level_niptr;
  };
};

struct list_ {
  struct list_v *set;
  int nums;
};

ks_finline
void list_init (struct list_ **St) {

  struct list_ *u = (struct list_ *) malloc (sizeof (struct list_));

  u->nums =0;
  u->set =ks_null;
  *St = u;
}

ks_finline
struct list_v *list_get (struct list_ *St, int pos) {

  int Q;
  struct list_v *U = ks_null;
  assert (St != ks_null);
  
  if (pos < 0) 
    return St->set->level_p;
  
  if (pos <= (St->nums - 1)) 
    for (Q =0, U = St->set; Q != pos; Q++) 
      U =U->level_n;
  return U;
}

ks_finline
void *list_getnode (struct list_ *St, int pos) {

  struct list_v *p;
  
  if ((p = list_get (St, pos)) != ks_null)
    return p->obj;
  else 
    return ks_null;
}

ks_finline
int list_insert (struct list_ *St, int pos, void *obj) {

  struct list_v *U = ks_null;
  struct list_v *V;
  assert (St != ks_null);

  if (pos < 0) {
    /* insert tail */
    U = (struct list_v *)  malloc (sizeof (struct list_v));
    U->obj = obj;

    if (St->nums == 0) {

      U->level_n = U;
      U->level_p = U;

      St->set = U;
    } else {
      U->level_n = St->set;
      U->level_p = St->set->level_p;
      St->set->level_p->level_n = U;
      St->set->level_p = U;
    }
    St->nums++;
    return 0;
  }
  if (pos > (St->nums))
    return -1;
  else ;

  U = (struct list_v *)  malloc (sizeof (struct list_v));
  U->obj = obj;

  if (St->nums == 0) {

    U->level_n = U;
    U->level_p = U;

    St->set = U;
    St->nums++;
    return 0;
  }

  V = list_get (St, pos == St->nums ?0: pos);

  U->level_n = V;
  U->level_p = V->level_p;

  V->level_p->level_n = U;
  V->level_p = U;

  if (pos ==0)
    St->set = U;
  St->nums++;
  return 0;
}

ks_finline
void *list_pop (struct list_ *St, int pos) {

  struct list_v *U = ks_null;
  struct list_v *V;
  void *obj;
  assert (St != ks_null);

  if (pos < 0)
    pos = St->nums - 1;
  else ;
  
  if (pos >= St->nums || (St->nums <= 0))
    return ks_null;
  else ;

  V = list_get (St,  pos);
  U = V;

  if (pos ==0) 
    St->set = V->level_n;
  
  /*  Prev->Next */
  V->level_p
   ->level_n = V->level_n;
  /*  Next->Prev */
  V->level_n
   ->level_p = V->level_p;

  if ( (-- St->nums) == 0)
    St->set = ks_null;
  obj = V->obj;
  
  free (V);
  return obj;
}

ks_finline
void list_remove_ISQ (struct list_ *St, int pos, void (*pernode_)(void **node)) {

  struct list_v *U = ks_null;
  struct list_v *V;
  assert (St != ks_null);

  if (pos == -1)
    pos = St->nums - 1;
  else ;

  if (pos >= (St->nums) || (St->nums <= 0))
    return ;
  else ;

  V = list_get (St,  pos);
  U = V;

  if (pos ==0) 
    St->set = V->level_n; 
  else 

  /*  Prev->Next */
  V->level_p
   ->level_n = V->level_n;
  /*  Next->Prev */
  V->level_n
   ->level_p = V->level_p;

  if (pernode_ != ks_null)
    pernode_ (& V->obj);
  else ;
  
  if ( (-- St->nums) == 0) 
    St->set = ks_null;
  free (V);
}

ks_finline
void list_remove (struct list_ *St, int pos) {

  list_remove_ISQ (St, pos, ks_null);
}

ks_finline
void list_uninit_ISQ (struct list_ **St, void (*pernode_)(void **node)) {

  if (St != ks_null && (*St != ks_null)) {
    while ( (*St)->nums > 0) 
      list_remove_ISQ (* St, 0, pernode_);
    free (*St); 
    * St = ks_null;
  }
}

ks_finline
void list_removeALL_ISQ (struct list_ *St, void (*pernode_)(void **node)) {

  if (St != ks_null) {
    while ( St->nums > 0) 
      list_remove_ISQ (St, 0, pernode_);
    St->set = ks_null;
  }
}

ks_finline
void list_uninit (struct list_ **St) {

  list_uninit_ISQ (St, ks_null);
}

/*
 *  not mine, from linux source,
 *  but it looks pretty bad in this version
 */
#define LIST_FOREACH(/* list_ * */ head, /* int*/ id_, /* list_v */ node)    \
  for (    (id_) = 0, (node) = (head)->set;                                  \
           (id_) != (head)->nums;                                            \
                   (node) = (node)->level_n,  (id_)++) 

#endif 