#include "joypad_buffer_deque.h"

/*** joypadbudffer queue  *****/
void jpbuffer_init (struct jpbuffer_section **pp) {

  struct jpbuffer_section *p= malloc(sizeof (struct jpbuffer_section));
  assert (p != null);

  p->adapter = null;
  p->count =0;
  p->root =null;
  p->tail =null;

  *pp =p;
}

void jpbuffer_push (struct jpbuffer_section *p, struct jpbuffer *desc) {

  struct jpbuffer_link *newblock = null;

  /* first, check free-list, get new block **/
  if (p->adapter != null) {
    newblock = p->adapter;
    p->adapter = p->adapter->level;
  } else {
    newblock = malloc (sizeof (struct jpbuffer_link));
    assert (newblock != null);
  }
  memcpy ( & newblock->desc, desc, sizeof (struct jpbuffer));
  newblock->level =null;

  /* insert tail **/
  if (p->tail != null) {
    p->tail->level = newblock;
  } else {
    p->root = newblock;
  }
  p->tail = newblock;
  p->count++;
}

void jpbuffer_pop (struct jpbuffer_section *p) {

  struct jpbuffer_link *release_node;

  if (p == null || (p->count <= 0)) {
    return;
  } else {
    release_node = p->root;
  }

  p->root = p->root->level;
  if (p->root == null) {
    p->tail = null;
  }
  p->count--;
  release_node->level = p->adapter;

  p->adapter = release_node;
}

void jpbuffer_throw (struct jpbuffer_section *p) {

  if (p == null || (p->count <= 0)) {
    return;
  } 

  p->tail->level = p->adapter;
  p->adapter = p->root;
  p->count = 0;
  p->tail = null;
  p->root = null;
}

void jpbuffer_destroy (struct jpbuffer_section *p) {

  struct jpbuffer_link *rps;
  struct jpbuffer_link *rpt;

  /* deal all node in root **/
  for (rpt =p->root; rpt != null; rpt = rps) {
    rps = rpt->level;
    free (rpt);
  }

  for (rpt =p->adapter; rpt != null; rpt = rps) {
    rps = rpt->level;
    free (rpt);
  }
}