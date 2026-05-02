#ifndef NRF_LOG_H__
#define NRF_LOG_H__

#include <stdint.h>
#include "elog.h"

#ifndef NRF_LOG_ENABLED
#define NRF_LOG_ENABLED 0
#endif

#if defined(PROJECT_IS_BL)
#define NRF_LOG_WRAPPER_TAG "bl"
#elif defined(PROJECT_IS_APP)
#define NRF_LOG_WRAPPER_TAG "app"
#else
#define NRF_LOG_WRAPPER_TAG "log"
#endif

#define NRF_LOG_MODULE_REGISTER(...)
#define NRF_LOG_MODULE_DECLARE(...)

#define NRF_LOG_INFO(...)    do { elog_i(NRF_LOG_WRAPPER_TAG, ##__VA_ARGS__); } while (0);
#define NRF_LOG_WARNING(...) do { elog_w(NRF_LOG_WRAPPER_TAG, ##__VA_ARGS__); } while (0);
#define NRF_LOG_ERROR(...)   do { elog_e(NRF_LOG_WRAPPER_TAG, ##__VA_ARGS__); } while (0);
#define NRF_LOG_DEBUG(...)   do { elog_d(NRF_LOG_WRAPPER_TAG, ##__VA_ARGS__); } while (0);
#define NRF_LOG_RAW_INFO(...)    do { elog_i(NRF_LOG_WRAPPER_TAG, ##__VA_ARGS__); } while (0);
#define NRF_LOG_RAW_WARNING(...) do { elog_w(NRF_LOG_WRAPPER_TAG, ##__VA_ARGS__); } while (0);
#define NRF_LOG_RAW_ERROR(...)   do { elog_e(NRF_LOG_WRAPPER_TAG, ##__VA_ARGS__); } while (0);
#define NRF_LOG_RAW_DEBUG(...)   do { elog_d(NRF_LOG_WRAPPER_TAG, ##__VA_ARGS__); } while (0);

#define NRF_LOG_HEXDUMP_INFO(p_data, len)    do { elog_hexdump(NRF_LOG_WRAPPER_TAG, 16, (p_data), (len)); } while (0);
#define NRF_LOG_HEXDUMP_WARNING(p_data, len) do { elog_hexdump(NRF_LOG_WRAPPER_TAG, 16, (p_data), (len)); } while (0);
#define NRF_LOG_HEXDUMP_ERROR(p_data, len)   do { elog_hexdump(NRF_LOG_WRAPPER_TAG, 16, (p_data), (len)); } while (0);
#define NRF_LOG_HEXDUMP_DEBUG(p_data, len)   do { elog_hexdump(NRF_LOG_WRAPPER_TAG, 16, (p_data), (len)); } while (0);

#define NRF_LOG_ERROR_STRING_GET(error_code) ""

#endif
