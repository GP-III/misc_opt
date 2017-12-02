#ifndef __JOYPAD_BUFFER_QUEUE_INC_
#define __JOYPAD_BUFFER_QUEUE_INC_

#include "type.h"
#include "8bit_host.h"
#include "frame.uuid.h"
#include "frame.cody.h"

struct jpbuffer {
  bool_t Alert_left;
  bool_t Alert_right;
  bool_t Alert_up;
  bool_t Alert_down;
  bool_t Alert_b;
  bool_t Alert_a;
  bool_t Alert_select;
  bool_t Alert_start; 
};

struct jpbuffer_link {
  struct jpbuffer_link *level;
  struct jpbuffer desc;
};

struct jpbuffer_section {
  struct jpbuffer_link *adapter;
  struct jpbuffer_link *root;
  struct jpbuffer_link *tail;
  int count;
  int jpbuffer_count;
};

void jpbuffer_init (struct jpbuffer_section **pp);
void jpbuffer_push (struct jpbuffer_section *p, struct jpbuffer *desc);
void jpbuffer_pop (struct jpbuffer_section *p);
void jpbuffer_throw (struct jpbuffer_section *p);
void jpbuffer_destroy (struct jpbuffer_section *p);

#endif 