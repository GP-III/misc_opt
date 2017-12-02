#ifndef __FRAME_QUEUE_INC_
#define __FRAME_QUEUE_INC_

#include "type.h"
#include "8bit_host.h"
#include "frame.uuid.h"
#include "frame.cody.h"

enum FDIR_ {
  FDIR_LEFT,
  FDIR_RIGHT,
  FDIR_NULL,
};

enum FRAME_UPDATE_ {
  FRAME_AUTO_UPDATE,
  FRAME_MAN_UPDATE,
  FRAME_CONTINUITY_UPDATE /* only suit same type **/
};

struct fqueue {

  enum FDIR_ dir;
  enum FRAME_UPDATE_ update_type;

  void *ext_struct;
  
  union {
	  CCACT_EVT evt_cc;
  };
  
  int ext_code;
  
  int count;
  int dispatch_count;
  
  int init_time;
  int poll_time;
  int ext_time;
  int man_time;
  int eafrva_time; 
  int rpt_time;
  int *continuity_time;
  int eiparam[20];

  void *fsm_object;
  
  void (*ctor_routine)(struct fqueue *, void *eparam);
  void *eparam_ctor;

  void (*entries_routine)(struct fqueue *, int ticks, void *eparam);
  void *eparam_entries;

  void (*entries2_routine)(struct fqueue *, int ticks, void *eparam);
  void *eparam_entries2;

  void (*dtor_routine)(struct fqueue *, void *eparam);
  void *eparam_dtor;

  void *evparam[20];

  bool_t isCtor;  /* assert init ***/
};

struct fqueue_link {
  struct fqueue_link *level;
  struct fqueue desc;
};

struct fqueue_section {
  struct fqueue_link *adapter;
  struct fqueue_link *root;
  struct fqueue_link *tail;
  int count;
  int frame_count;
  
  frame_uuid uuid;
};

void fqueue_init (struct fqueue_section **pp);
void fqueue_push (struct fqueue_section *p, struct fqueue *desc);
void fqueue_pop (struct fqueue_section *p);
void fqueue_throw (struct fqueue_section *p);
void fqueue_destroy (struct fqueue_section *p);

#endif