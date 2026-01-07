#ifndef UNIT_STUBS_H
#define UNIT_STUBS_H

/* Ensure math functions like fabs have proper prototypes */
#include <math.h>

/* Minimal ThreadX type aliases used in production code to satisfy compiler */
typedef unsigned long ULONG;
typedef void VOID;

typedef unsigned int UINT;

/* Forward-declare primitive RTOS structs used for references */
typedef struct { unsigned long flags; } TX_EVENT_FLAGS_GROUP;
typedef struct { int dummy; } TX_QUEUE;

/* Common RTOS constants used in code paths (values are placeholders for tests) */
#define TX_SUCCESS ((UINT)0)
#define TX_NO_WAIT ((ULONG)0)
#define TX_OR_CLEAR ((UINT)0)

/* Prototypes to avoid -Wmissing-prototypes (definitions provided in threadx_stubs.c) */
UINT tx_event_flags_get(TX_EVENT_FLAGS_GROUP *group,
                        ULONG requested,
                        UINT get_option,
                        ULONG *actual_flags,
                        ULONG wait_option);

UINT tx_queue_receive(TX_QUEUE *queue,
                      void *source_ptr,
                      ULONG wait_option);

VOID tx_thread_sleep(ULONG timer_ticks);

/* Extern variables referenced by production code */
extern TX_EVENT_FLAGS_GROUP event_flags;
extern TX_QUEUE queue_speed_cmd;

#endif /* UNIT_STUBS_H */
