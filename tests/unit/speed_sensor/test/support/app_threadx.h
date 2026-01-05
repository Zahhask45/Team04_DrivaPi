#ifndef APP_THREADX_H
#define APP_THREADX_H

#include <stdint.h>

// Define types ThreadX uses
typedef unsigned long ULONG;
typedef unsigned int  UINT;

// Define Macros
#ifndef TX_TIMER_TICKS_PER_SECOND
#define TX_TIMER_TICKS_PER_SECOND 1000
#endif

#define TX_WAIT_FOREVER 0xFFFFFFFF
#define TX_OR           0
#define TX_SUCCESS      0

// Function Prototypes for CMock
ULONG tx_time_get(void);
UINT  tx_thread_sleep(ULONG timer_ticks);

// ThreadX Structs
typedef struct { int dummy; } TX_MUTEX;
typedef struct { int dummy; } TX_EVENT_FLAGS_GROUP;

UINT tx_mutex_get(TX_MUTEX *mutex_ptr, ULONG wait_option);
UINT tx_mutex_put(TX_MUTEX *mutex_ptr);
UINT tx_event_flags_set(TX_EVENT_FLAGS_GROUP *group_ptr, ULONG flags_to_set, UINT set_option);

#endif
