/*
MIT License

Copyright (c) 2025 頔珞JaderoChan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef KBDT_H
#define KBDT_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif // _cplusplus

#ifdef _MSC_VER
    #ifdef KBDT_BUILD_SHARED
        #define KBDT_API __declspec(dllexport)
    #elif defined(KBDT_SHARED)
        #define KBDT_API __declspec(dllimport)
    #else
        #define KBDT_API
    #endif // KBDT_BUILD_SHARED
#else
    #define KBDT_API __attribute__((visibility("default")))
#endif // _WIN32

#define KBDT_RC_SUCCESS     0
#define KBDT_RC_FAIL       -1

enum keyboard_event_type
{
    KBDET_RELEASED,
    KBDET_PRESSED
};

struct keyboard_event
{
    enum keyboard_event_type type;
    int native_key;
};

typedef void (*keyboard_event_handler)(struct keyboard_event*);

KBDT_API int kbdt_run();

/**
 * @attention Do not perform this function in the thread of the event handler.
 */
KBDT_API int kbdt_end();

KBDT_API int kbdt_set_event_handler(keyboard_event_handler handler);

/**
 * @note All events sent through this function or similar functions will not be handled by the event handler.
 * @return The number of the events sent.
 */
KBDT_API size_t kbdt_send_events(struct keyboard_event* events, size_t event_count);

KBDT_API size_t kbdt_send_event(struct keyboard_event event);

/**
 * @return The number of the events added to buffer.
 */
KBDT_API size_t kbdt_add_events_to_buf(struct keyboard_event* events, size_t event_count);

KBDT_API size_t kbdt_add_event_to_buf(struct keyboard_event event);

KBDT_API void kbdt_clear_event_buf();

/**
 * @brief Send all the events in the buffer but not clear the buffer.
 * @return The number of the events sent.
 */
KBDT_API size_t kbdt_send_event_buf();

/**
 * @brief Send all the events in the buffer and clear the buffer.
 * @return The number of the events flushed.
 */
KBDT_API size_t kbdt_flush_event_buf();

KBDT_API bool kbdt_is_running();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !KBDT_H
