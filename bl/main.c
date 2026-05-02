#include <stdint.h>

#include "app_error.h"
#include "app_error_weak.h"
#include "nrf_bootloader.h"
#include "nrf_bootloader_app_start.h"
#include "nrf_bootloader_info.h"
#include "nrf_mbr.h"

static void on_error(void)
{
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
}

int main(void)
{
    uint32_t ret_val;

    nrf_bootloader_mbr_addrs_populate();

    ret_val = nrf_bootloader_flash_protect(0, MBR_SIZE);
    APP_ERROR_CHECK(ret_val);
    ret_val = nrf_bootloader_flash_protect(BOOTLOADER_START_ADDR, BOOTLOADER_SIZE);
    APP_ERROR_CHECK(ret_val);

    ret_val = nrf_bootloader_init(dfu_observer);
    APP_ERROR_CHECK(ret_val);
    APP_ERROR_CHECK_BOOL(false);
}
