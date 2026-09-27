#pragma once
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// 啟動手機連線配置入口 (Web Dashboard Server)
esp_err_t web_config_portal_start(void);
void web_config_portal_stop(void);

#ifdef __cplusplus
}
#endif
