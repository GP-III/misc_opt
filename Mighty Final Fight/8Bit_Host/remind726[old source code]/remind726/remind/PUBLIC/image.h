#ifndef __video_
#define __video_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

int alloc_texure (void *vm, const unsigned char *name, unsigned char slot);
void free_texure (void *vm, unsigned char slot);
void *detail_texture (void *vm, unsigned char slot);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif