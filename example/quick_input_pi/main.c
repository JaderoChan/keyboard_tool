#include <stdio.h>
#include <stdlib.h> // exit

#include <pthread.h>

#include <kbdt/kbdt.h>
#include <kbdt/utility.h>

static bool g_should_close = false;
static pthread_mutex_t g_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cv_should_close = PTHREAD_COND_INITIALIZER;

static enum keyboard_key key_seq[2] = {0};

static void input_pi()
{
    static enum keyboard_key pi[] = {
        KBDKEY_3, KBDKEY_PERIOD, KBDKEY_1,
        KBDKEY_4, KBDKEY_1, KBDKEY_5,
        KBDKEY_9, KBDKEY_2, KBDKEY_6
    };
    static size_t size = sizeof(pi) / sizeof(enum keyboard_key);
    static bool is_inited = false;

    if (!is_inited)
    {
        struct keyboard_event event;
        for (size_t i = 0; i < size; ++i)
        {
            event.native_key = kbdt_keyboard_key_to_native(pi[i]);
            event.type = KBDET_PRESSED;
            kbdt_add_events_to_buf(&event, 1);
            event.type = KBDET_RELEASED;
            kbdt_add_events_to_buf(&event, 1);
        }
        is_inited = true;
    }
    kbdt_send_event_buf();
}

static void event_handler(struct keyboard_event* event)
{
    enum keyboard_key key = kbdt_keyboard_key_from_native(event->native_key);
    switch (event->type)
    {
        case KBDET_PRESSED:
            key_seq[0] = key_seq[1];
            key_seq[1] = key;
            if (key == KBDKEY_ESC)
            {
                pthread_mutex_lock(&g_mtx);
                g_should_close = true;
                pthread_cond_signal(&g_cv_should_close);
                pthread_mutex_unlock(&g_mtx);
            }
            break;
        case KBDET_RELEASED:
            if (key_seq[0] == KBDKEY_P && key_seq[1] == KBDKEY_I)
                input_pi();
            break;
        default:
            break;
    }
}

int main()
{
    int rc = kbdt_run();
    if (rc != KBDT_RC_SUCCESS)
    {
        printf("Failed to run kbdtool. Error code: %d\n", rc);
        exit(1);
    }

    rc = kbdt_set_event_handler(&event_handler);
    if (rc != KBDT_RC_SUCCESS)
    {
        printf("Failed to set event handler. Error code: %d\n", rc);
        exit(1);
    }

    printf("Press the 'p' and 'i' key to input 3.1415926.\n");
    printf("Press ESC to exit.\n\n");

    pthread_mutex_lock(&g_mtx);
    while (!g_should_close)
        pthread_cond_wait(&g_cv_should_close, &g_mtx);
    pthread_mutex_unlock(&g_mtx);

    kbdt_end();

    printf("Success to exit\n");
    return 0;
}
