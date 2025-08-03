#ifndef KBDT_PRIVATE_H
#define KBDT_PRIVATE_H

#include <kbdt/kbdt.h>

void set_run_success();

void set_run_fail(int error_code);

// The implementationa of the follow fucntion depend on platform.

/**
 * @note This function will be performed before the worker thread is running.
 */
int do_before_start_work();

/**
 * @note This function will be performed before the worker thread is end.
 * Specifically, only when this function returns will the thread to exit.
 */
int do_before_end_work();

/**
 * @attention The `set_run_success` or `set_run_fail` must be performed in
 * this function to indicate whether the work running successfully.
 */
void work();

int set_event_handler_p(keyboard_event_handler handler);

size_t send_events_p(struct keyboard_event* events, size_t event_count);

#endif // !KBDT_PRIVATE_H
