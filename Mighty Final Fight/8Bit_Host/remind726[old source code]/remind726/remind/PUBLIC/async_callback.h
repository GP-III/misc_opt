#ifndef __async_callback_
#define __async_callback_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

void *init_async_callback (void);
void uninit_async_callback (void *cbm);
/* callback-register Not suitable for ILP64 */
void async_callback_register_stream_end (void *cbm, void (*stream_end_)(void));
void async_callback_register_pass_end (void *cbm, void (*pass_end_)(void));
void async_callback_register_pass_start (void *cbm, void (*pass_start_)(unsigned int));
void async_callback_register_buffer_end (void *cbm, void (*buffer_end_)(void *));
void async_callback_register_buffer_start (void *cbm, void (*buffer_start_)(void *));
void async_callback_register_loop_end (void *cbm, void (*loop_end_)(void *));
void async_callback_register_event_error (void *cbm,  void (*event_error_)(void *, int));
void async_callback_register_callback_destroy (void *cbm, void (*callback_destroy_)(void));
/* callback-wait Not suitable for ILP64 */
int async_callback_wait_stream_end (void *cbm, unsigned int wait_time);
int async_callback_wait_pass_end (void *cbm, unsigned int wait_time);
int async_callback_wait_pass_start (void *cbm, unsigned int wait_time);
int async_callback_wait_buffer_end (void *cbm, unsigned int wait_time);
int async_callback_wait_buffer_start (void *cbm, unsigned int wait_time);
int async_callback_wait_loop_end (void *cbm, unsigned int wait_time);
int async_callback_wait_event_error (void *cbm, unsigned int wait_time);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif