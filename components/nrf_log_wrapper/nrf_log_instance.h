#ifndef NRF_LOG_INSTANCE_H__
#define NRF_LOG_INSTANCE_H__

#include <stdint.h>

typedef struct
{
    uint8_t dummy;
} nrf_log_instance_t;

#define NRF_LOG_INSTANCE_PTR_DECLARE(_p_name)
#define NRF_LOG_INSTANCE_REGISTER(_module_name, _inst_name, _info_color, _debug_color, _initial_lvl, _compiled_lvl)
#define NRF_LOG_INSTANCE_PTR_INIT(_p_name, _module_name, _inst_name)

#define NRF_LOG_INST_INFO(p_inst, ...)
#define NRF_LOG_INST_WARNING(p_inst, ...)
#define NRF_LOG_INST_ERROR(p_inst, ...)
#define NRF_LOG_INST_DEBUG(p_inst, ...)

#endif
