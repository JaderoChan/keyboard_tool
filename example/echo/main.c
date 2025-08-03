#include <stdio.h>
#include <stdlib.h> // exit

#include <pthread.h>

#include <kbdt/kbdt.h>
#include <kbdt/utility.h>

static bool g_should_close = false;
static pthread_mutex_t g_mtx;
static pthread_cond_t g_cv_should_close;

static void event_handler(struct keyboard_event* event)
{
    enum keyboard_key key = kbdt_keyboard_key_from_native(event->native_key);
    switch (event->type)
    {
        case KBDET_PRESSED:
            printf("Key pressed: %s\n", kbdt_keyboard_key_to_str(key));
            if (key == KBDKEY_ESC)
            {
                pthread_mutex_lock(&g_mtx);
                g_should_close = true;
                pthread_mutex_unlock(&g_mtx);
                pthread_cond_signal(&g_cv_should_close);
            }
            break;
        case KBDET_RELEASED:
            printf("Key released: %s\n",  kbdt_keyboard_key_to_str(key));
            break;
        default:
            break;
    }
}

int main()
{
    pthread_mutex_init(&g_mtx, NULL);
    pthread_cond_init(&g_cv_should_close, NULL);

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

    printf("Press ESC to exit.\n\n");

    pthread_mutex_lock(&g_mtx);
    while (!g_should_close)
        pthread_cond_wait(&g_cv_should_close, &g_mtx);
    pthread_mutex_unlock(&g_mtx);

    printf("Success to exit\n");
    return 0;
}
