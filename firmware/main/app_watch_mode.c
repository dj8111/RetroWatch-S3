#include "retrowatch_common.h"
#include "bsp_audio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "APP_WATCH";

void app_watch_mode_init(void) {
    ESP_LOGI(TAG, "Entering Watch & Finance Ticker Mode (NTP Clock + Yahoo API)");
    // 插電時 5 秒跳錶一次，電池供電時 60 秒跳錶一次
}

void app_watch_mode_handle_input(btn_event_t evt) {
    switch (evt) {
        case BTN_EVT_UP:
        case BTN_EVT_DOWN:
            bsp_audio_button_tick();
            ESP_LOGI(TAG, "Switching Watch Ticker Card (AAPL -> NVDA -> BTC -> ETH)");
            break;
        case BTN_EVT_A_CLICK:
            bsp_audio_button_tick();
            ESP_LOGI(TAG, "Manual Force Refresh Quotes");
            break;
        case BTN_EVT_CENTER:
        case BTN_EVT_START_CLICK:
            retrowatch_switch_mode(SYS_MODE_MENU);
            break;
        default:
            break;
    }
}
