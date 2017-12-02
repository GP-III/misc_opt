#include "frame.queue.h"

/*** frame queue  *****/
void fqueue_init (struct fqueue_section **pp) {

  struct fqueue_section *p= malloc(sizeof (struct fqueue_section));
  assert (p != null);

  p->adapter = null;
  p->count =0;
  p->root =null;
  p->tail =null;

  *pp =p;
}

void fqueue_push (struct fqueue_section *p, struct fqueue *desc) {

  struct fqueue_link *newblock = null;

  /* first, check free-list, get new block **/
  if (p->adapter != null) {
    newblock = p->adapter;
    p->adapter = p->adapter->level;
  } else {
    newblock = malloc (sizeof (struct fqueue_link));
    assert (newblock != null);
  }
  memcpy ( & newblock->desc, desc, sizeof (struct fqueue));
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

void fqueue_pop (struct fqueue_section *p) {

  struct fqueue_link *release_node;

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

void fqueue_throw (struct fqueue_section *p) {

  if (p == null || (p->count <= 0)) {
    return;
  } 

  p->tail->level = p->adapter;
  p->adapter = p->root;
  p->count = 0;
  p->tail = null;
  p->root = null;
}

void fqueue_destroy (struct fqueue_section *p) {

  struct fqueue_link *rps;
  struct fqueue_link *rpt;

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