#ifndef __video_
#define __video_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

int init_video (void *sys);
int uninit_video (void *vm);
int video_render (void *vm, unsigned int *vbuf, unsigned int pitch);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif