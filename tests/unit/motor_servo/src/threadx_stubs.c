#include "../test/support/unit_stubs.h"
#include <string.h>

/* Provide minimal stub definitions so linker can resolve symbols from dc_motor.c */

TX_EVENT_FLAGS_GROUP event_flags = {0};
TX_QUEUE queue_speed_cmd = {0};

UINT tx_event_flags_get(TX_EVENT_FLAGS_GROUP *group,
                        ULONG requested,
                        UINT get_option,
                        ULONG *actual_flags,
                        ULONG wait_option)
{
    (void)group;
    (void)requested;
    (void)get_option;
    (void)wait_option;
    if (actual_flags) { *actual_flags = 0; }
    return TX_SUCCESS;
}

UINT tx_queue_receive(TX_QUEUE *queue,
                      void *source_ptr,
                      ULONG wait_option)
{
    (void)queue;
    (void)source_ptr;
    (void)wait_option;
    /* For unit tests, simulate empty queue to avoid loops */
    return TX_SUCCESS != 0 ? TX_SUCCESS : 1; /* return non-success to break loops */
}

VOID tx_thread_sleep(ULONG timer_ticks)
{
    (void)timer_ticks;
    /* No-op in unit tests */
}
