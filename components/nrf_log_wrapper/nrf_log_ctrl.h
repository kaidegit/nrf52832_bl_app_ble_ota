#ifndef NRF_LOG_CTRL_H__
#define NRF_LOG_CTRL_H__

#include <stdbool.h>
#include <stdint.h>
#include "elog.h"

#define NRF_LOG_FINAL_FLUSH()      do { } while (0)
#define NRF_LOG_PROCESS()          false
#define NRF_LOG_PENDING()          false
#define NRF_LOG_FLUSH()            do { } while (0)
#define NRF_LOG_PANIC()            do { } while (0)
#define NRF_LOG_INIT(timestamp_fn) ((void)(timestamp_fn), elog_init(), elog_start(), 0U)

#endif
