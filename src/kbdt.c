#include <kbdt/kbdt.h>

#include <memory.h>     // memcpy
#include <stdlib.h>     // malloc, free

#include <pthread.h>

#include "kbdt_private.h"

static pthread_t g_worker_thread;
static pthread_mutex_t g_mtx = PTHREAD_MUTEX_INITIALIZER;
#define LOCK(mtx)   pthread_mutex_lock(mtx)
#define UNLOCK(mtx) pthread_mutex_unlock(mtx)

enum running_state
{
    RS_FREE,
    RS_RUNNING,
    RS_TERMINATE
};
static pthread_cond_t g_cv_running_state = PTHREAD_COND_INITIALIZER;
static enum running_state g_running_state = RS_FREE;
static int g_running_rc = -1;

#define EVENT_BUFFER_SIZE 128
static struct keyboard_event g_event_buffer[EVENT_BUFFER_SIZE];
static size_t g_event_buffer_size = 0;

static void* thread_work(void* arg)
{
    work();
    LOCK(&g_mtx);
    // If the work is exit normally (the running state is `RS_RUNNING`) set the running state to `RS_FREE`,
    // else if the work is exit in error (the running state is `RS_TERMINATE`) keep the running state.
    if (g_running_state == RS_RUNNING)
        g_running_state = RS_FREE;
    pthread_cond_signal(&g_cv_running_state);
    UNLOCK(&g_mtx);
    return NULL;
}

KBDT_API int kbdt_run()
{
    if (kbdt_is_running())
        return KBDT_RC_SUCCESS;

    int rc = do_before_start_work();
    if (rc != KBDT_RC_SUCCESS)
        return rc;

    pthread_create(&g_worker_thread, NULL, &thread_work, NULL);
    pthread_detach(g_worker_thread);

    LOCK(&g_mtx);
    // Wait the work set the running state.
    while (g_running_state == RS_FREE)
        pthread_cond_wait(&g_cv_running_state, &g_mtx);

    // If the work occur error set the running state to `RS_FREE` and clear related resource.
    if (g_running_state == RS_TERMINATE)
        g_running_state = RS_FREE;
    rc = g_running_rc;
    UNLOCK(&g_mtx);

    return rc;
}

KBDT_API int kbdt_end()
{
    if (!kbdt_is_running())
        return KBDT_RC_SUCCESS;

    int rc = do_before_end_work();
    if (rc != KBDT_RC_SUCCESS)
        return rc;

    LOCK(&g_mtx);
    // Wait the work is exit.
    while (g_running_state != RS_FREE)
        pthread_cond_wait(&g_cv_running_state, &g_mtx);
    UNLOCK(&g_mtx);

    return KBDT_RC_SUCCESS;
}

KBDT_API int kbdt_set_event_handler(keyboard_event_handler handler)
{
    return set_event_handler_p(handler);
}

KBDT_API size_t kbdt_send_events(struct keyboard_event* events, size_t event_count)
{
    return send_events_p(events, event_count);
}

KBDT_API size_t kbdt_send_event(struct keyboard_event event)
{
    return kbdt_send_events(&event, 1);
}

KBDT_API size_t kbdt_add_events_to_buf(struct keyboard_event* events, size_t event_count)
{
    LOCK(&g_mtx);
    if (g_event_buffer_size + event_count > EVENT_BUFFER_SIZE)
        event_count = EVENT_BUFFER_SIZE - g_event_buffer_size;
    memcpy(&g_event_buffer[g_event_buffer_size], events, event_count * sizeof(struct keyboard_event));
    g_event_buffer_size += event_count;
    UNLOCK(&g_mtx);
    return event_count;
}

KBDT_API size_t kbdt_add_event_to_buf(struct keyboard_event event)
{
    return kbdt_add_events_to_buf(&event, 1);
}

KBDT_API void kbdt_clear_event_buf()
{
    LOCK(&g_mtx);
    g_event_buffer_size = 0;
    UNLOCK(&g_mtx);
}

KBDT_API size_t kbdt_send_event_buf()
{
    size_t sent = 0;
    LOCK(&g_mtx);
    sent = send_events_p(g_event_buffer, g_event_buffer_size);
    UNLOCK(&g_mtx);
    return sent;
}

KBDT_API size_t kbdt_flush_event_buf()
{
    size_t sent = 0;
    LOCK(&g_mtx);
    sent = send_events_p(g_event_buffer, g_event_buffer_size);
    g_event_buffer_size = 0;
    UNLOCK(&g_mtx);
    return sent;
}

KBDT_API bool kbdt_is_running()
{
    LOCK(&g_mtx);
    bool is_running = g_running_state == RS_RUNNING;
    UNLOCK(&g_mtx);
    return is_running;
}

void set_run_success()
{
    LOCK(&g_mtx);
    g_running_rc = KBDT_RC_SUCCESS;
    g_running_state = RS_RUNNING;
    pthread_cond_signal(&g_cv_running_state);
    UNLOCK(&g_mtx);
}

void set_run_fail(int error_code)
{
    LOCK(&g_mtx);
    g_running_rc = error_code;
    g_running_state = RS_TERMINATE;
    UNLOCK(&g_mtx);
}
