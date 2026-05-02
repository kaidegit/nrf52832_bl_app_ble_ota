#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "app_error_weak.h"
#include "nrf_bootloader.h"
#include "nrf_bootloader_app_start.h"
#include "nrf_bootloader_info.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_mbr.h"
#include "SEGGER_RTT.h"
#include "../src/elog_port.h"

static bool m_rtt_initialized;
static bool m_rtt_reused;

static bool rtt_early_init(void)
{
    if (!m_rtt_initialized)
    {
        m_rtt_reused = elog_port_rtt_init();
        m_rtt_initialized = true;
    }

    return m_rtt_reused;
}

static void log_init(void)
{
    bool rtt_reused = rtt_early_init();
    SEGGER_RTT_WriteString(0, "[BL] log_init enter\r\n");
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    NRF_LOG_INFO("RTT %s", rtt_reused ? "reused across image jump" : "initialized");
    SEGGER_RTT_WriteString(0, "[BL] log_init done\r\n");
}

static void on_error(void)
{
    NRF_LOG_ERROR("Fatal error, resetting");
    NVIC_SystemReset();
}

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    UNUSED_PARAMETER(error_code);
    UNUSED_PARAMETER(line_num);
    UNUSED_PARAMETER(p_file_name);
    on_error();
}

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
    UNUSED_PARAMETER(id);
    UNUSED_PARAMETER(pc);
    UNUSED_PARAMETER(info);
    on_error();
}

void app_error_handler_bare(uint32_t error_code)
{
    UNUSED_PARAMETER(error_code);
    on_error();
}

static void dfu_observer(nrf_dfu_evt_type_t evt_type)
{
    UNUSED_PARAMETER(evt_type);
    NRF_LOG_INFO("DFU event");
}

int main(void)
{
    (void)rtt_early_init();
    SEGGER_RTT_WriteString(0, "[BL] main enter\r\n");
    uint32_t ret_val;

    log_init();
    NRF_LOG_INFO("Bootloader entry");

    nrf_bootloader_mbr_addrs_populate();
    NRF_LOG_INFO("MBR addresses populated");

    ret_val = nrf_bootloader_flash_protect(0, MBR_SIZE);
    APP_ERROR_CHECK(ret_val);
    ret_val = nrf_bootloader_flash_protect(BOOTLOADER_START_ADDR, BOOTLOADER_SIZE);
    APP_ERROR_CHECK(ret_val);
    NRF_LOG_INFO("Flash protection configured");

    NRF_LOG_INFO("Starting DFU core");
    ret_val = nrf_bootloader_init(dfu_observer);
    APP_ERROR_CHECK(ret_val);
    APP_ERROR_CHECK_BOOL(false);
}
