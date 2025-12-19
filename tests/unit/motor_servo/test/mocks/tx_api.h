/**
 * @file tx_api.h
 * @brief ThreadX RTOS API Mock Header for Unit Testing
 * @note This header allows testing ThreadX applications on Linux x64 without real RTOS
 * 
 * CMock will generate:
 * - mock_tx_api.c
 * - mock_tx_api.h
 * 
 * Usage in tests:
 * #include "mock_tx_api.h"
 * 
 * @copyright Microsoft ThreadX
 * @note Simplified for unit testing purposes
 */

#ifndef TX_API_H
#define TX_API_H

#include <stdint.h>

/* ========================================================================
 * THREADX TYPE DEFINITIONS
 * ======================================================================== */

typedef uint32_t UINT;
typedef uint32_t ULONG;
typedef void VOID;
typedef char CHAR;

/* Thread Control Block */
typedef struct TX_THREAD_STRUCT {
    ULONG   tx_thread_id;
    CHAR    *tx_thread_name;
    UINT    tx_thread_state;
    UINT    tx_thread_priority;
    ULONG   tx_thread_run_count;
    VOID    *tx_thread_stack_start;
    ULONG   tx_thread_stack_size;
} TX_THREAD;

/* Queue Control Block */
typedef struct TX_QUEUE_STRUCT {
    ULONG   tx_queue_id;
    CHAR    *tx_queue_name;
    UINT    tx_queue_message_size;
    ULONG   tx_queue_capacity;
    ULONG   tx_queue_available_storage;
    ULONG   tx_queue_enqueued;
} TX_QUEUE;

/* Semaphore Control Block */
typedef struct TX_SEMAPHORE_STRUCT {
    ULONG   tx_semaphore_id;
    CHAR    *tx_semaphore_name;
    ULONG   tx_semaphore_count;
} TX_SEMAPHORE;

/* Mutex Control Block */
typedef struct TX_MUTEX_STRUCT {
    ULONG     tx_mutex_id;
    CHAR      *tx_mutex_name;
    UINT      tx_mutex_ownership_count;
    TX_THREAD *tx_mutex_owner;
} TX_MUTEX;

/* Event Flags Group */
typedef struct TX_EVENT_FLAGS_GROUP_STRUCT {
    ULONG   tx_event_flags_id;
    CHAR    *tx_event_flags_name;
    ULONG   tx_event_flags_current;
} TX_EVENT_FLAGS_GROUP;

/* ========================================================================
 * THREADX RETURN CODES
 * ======================================================================== */

#define TX_SUCCESS                  0x00
#define TX_DELETED                  0x01
#define TX_NO_MEMORY                0x10
#define TX_POOL_ERROR               0x02
#define TX_PTR_ERROR                0x03
#define TX_WAIT_ERROR               0x04
#define TX_SIZE_ERROR               0x05
#define TX_GROUP_ERROR              0x06
#define TX_NO_EVENTS                0x07
#define TX_OPTION_ERROR             0x08
#define TX_QUEUE_ERROR              0x09
#define TX_QUEUE_EMPTY              0x0A
#define TX_QUEUE_FULL               0x0B
#define TX_SEMAPHORE_ERROR          0x0C
#define TX_NO_INSTANCE              0x0D
#define TX_THREAD_ERROR             0x0E
#define TX_PRIORITY_ERROR           0x0F
#define TX_SUSPEND_ERROR            0x14
#define TX_WAIT_ABORTED             0x1A
#define TX_MUTEX_ERROR              0x1C

/* ========================================================================
 * THREADX OPTIONS
 * ======================================================================== */

#define TX_NO_WAIT                  0x00000000UL
#define TX_WAIT_FOREVER             0xFFFFFFFFUL
#define TX_AUTO_START               1
#define TX_DONT_START               0
#define TX_NO_TIME_SLICE            0

/* ========================================================================
 * THREADX API FUNCTIONS (Will be mocked by CMock)
 * ======================================================================== */

/* Thread Management */
UINT tx_thread_create(TX_THREAD *thread_ptr, CHAR *name_ptr,
                      VOID (*entry_function)(ULONG), ULONG entry_input,
                      VOID *stack_start, ULONG stack_size,
                      UINT priority, UINT preempt_threshold,
                      ULONG time_slice, UINT auto_start);

UINT tx_thread_delete(TX_THREAD *thread_ptr);
UINT tx_thread_suspend(TX_THREAD *thread_ptr);
UINT tx_thread_resume(TX_THREAD *thread_ptr);
VOID tx_thread_sleep(ULONG timer_ticks);
VOID tx_thread_relinquish(VOID);

/* Queue Management */
UINT tx_queue_create(TX_QUEUE *queue_ptr, CHAR *name_ptr,
                     UINT message_size, VOID *queue_start, ULONG queue_size);

UINT tx_queue_send(TX_QUEUE *queue_ptr, VOID *source_ptr, ULONG wait_option);
UINT tx_queue_receive(TX_QUEUE *queue_ptr, VOID *destination_ptr, ULONG wait_option);
UINT tx_queue_flush(TX_QUEUE *queue_ptr);
UINT tx_queue_delete(TX_QUEUE *queue_ptr);

/* Semaphore Management */
UINT tx_semaphore_create(TX_SEMAPHORE *semaphore_ptr, CHAR *name_ptr,
                         ULONG initial_count);

UINT tx_semaphore_get(TX_SEMAPHORE *semaphore_ptr, ULONG wait_option);
UINT tx_semaphore_put(TX_SEMAPHORE *semaphore_ptr);
UINT tx_semaphore_delete(TX_SEMAPHORE *semaphore_ptr);

/* Mutex Management */
UINT tx_mutex_create(TX_MUTEX *mutex_ptr, CHAR *name_ptr, UINT inherit);
UINT tx_mutex_get(TX_MUTEX *mutex_ptr, ULONG wait_option);
UINT tx_mutex_put(TX_MUTEX *mutex_ptr);
UINT tx_mutex_delete(TX_MUTEX *mutex_ptr);

/* Event Flags */
UINT tx_event_flags_create(TX_EVENT_FLAGS_GROUP *group_ptr, CHAR *name_ptr);
UINT tx_event_flags_set(TX_EVENT_FLAGS_GROUP *group_ptr, ULONG flags_to_set,
                        UINT set_option);
UINT tx_event_flags_get(TX_EVENT_FLAGS_GROUP *group_ptr, ULONG requested_flags,
                        UINT get_option, ULONG *actual_flags_ptr,
                        ULONG wait_option);

#endif /* TX_API_H */
