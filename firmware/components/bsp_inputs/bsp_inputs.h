#pragma once
#include "esp_err.h"
#include "retrowatch_common.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t bsp_inputs_init(void);
btn_event_t bsp_inputs_poll(void);

#ifdef __cplusplus
}
#endif
