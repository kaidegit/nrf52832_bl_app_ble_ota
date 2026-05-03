#include <stdio.h>
#include <string.h>

#include "SEGGER_RTT.h"
#include "elog.h"
#include "elog_port.h"
#include "nrf.h"

#ifdef FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#endif

#ifdef FREERTOS
static SemaphoreHandle_t m_output_mutex;
#endif

static uint32_t m_irq_lock_primask;
static uint32_t m_irq_lock_nesting;

static bool rtt_control_block_valid(void)
{
    extern SEGGER_RTT_CB _SEGGER_RTT;
    static const char rtt_id[16] = "SEGGER RTT";

    return memcmp(_SEGGER_RTT.acID, rtt_id, sizeof(rtt_id)) == 0;
}

static void irq_lock(void)
{
    uint32_t primask = __get_PRIMASK();

    __disable_irq();
    if (m_irq_lock_nesting++ == 0)
    {
        m_irq_lock_primask = primask;
    }
}

static void irq_unlock(void)
{
    if ((m_irq_lock_nesting > 0) && (--m_irq_lock_nesting == 0))
    {
        if (m_irq_lock_primask == 0)
        {
            __enable_irq();
        }
    }
}

#ifdef FREERTOS
static bool rtos_lock_available(void)
{
    return (m_output_mutex != NULL) &&
           (__get_IPSR() == 0) &&
           (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING);
}
#endif

bool elog_port_rtt_init(void)
{
    extern SEGGER_RTT_CB _SEGGER_RTT;
    bool rtt_ready = rtt_control_block_valid();

    if (!rtt_ready)
    {
        memset(&_SEGGER_RTT, 0, sizeof(_SEGGER_RTT));
        SEGGER_RTT_Init();
    }

    return rtt_ready;
}

ElogErrCode elog_port_init(void)
{
#ifdef FREERTOS
    if (m_output_mutex == NULL)
    {
        m_output_mutex = xSemaphoreCreateRecursiveMutex();
    }
#endif

    (void)elog_port_rtt_init();
    return ELOG_NO_ERR;
}

void elog_port_deinit(void)
{
#ifdef FREERTOS
    if (m_output_mutex != NULL)
    {
        vSemaphoreDelete(m_output_mutex);
        m_output_mutex = NULL;
    }
#endif
}

void elog_port_output(const char *log, size_t size)
{
    SEGGER_RTT_Write(0, log, size);
}

void elog_port_output_lock(void)
{
#ifdef FREERTOS
    if (rtos_lock_available())
    {
        (void)xSemaphoreTakeRecursive(m_output_mutex, portMAX_DELAY);
        return;
    }
#endif

    irq_lock();
}

void elog_port_output_unlock(void)
{
#ifdef FREERTOS
    if (rtos_lock_available())
    {
        (void)xSemaphoreGiveRecursive(m_output_mutex);
        return;
    }
#endif

    irq_unlock();
}

const char *elog_port_get_time(void)
{
    static char time_buf[16];
    static uint32_t tick;
    (void)snprintf(time_buf, sizeof(time_buf), "%lu", (unsigned long)tick++);
    return time_buf;
}

const char *elog_port_get_p_info(void)
{
    return "";
}

const char *elog_port_get_t_info(void)
{
    return "";
}
