#include <stdio.h>
#include <string.h>

#include "SEGGER_RTT.h"
#include "elog.h"
#include "elog_port.h"

static bool rtt_control_block_valid(void)
{
    extern SEGGER_RTT_CB _SEGGER_RTT;
    static const char rtt_id[16] = "SEGGER RTT";

    return memcmp(_SEGGER_RTT.acID, rtt_id, sizeof(rtt_id)) == 0;
}

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
    (void)elog_port_rtt_init();
    return ELOG_NO_ERR;
}

void elog_port_deinit(void)
{
}

void elog_port_output(const char *log, size_t size)
{
    SEGGER_RTT_Write(0, log, size);
}

void elog_port_output_lock(void)
{
    __disable_irq();
}

void elog_port_output_unlock(void)
{
    __enable_irq();
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
