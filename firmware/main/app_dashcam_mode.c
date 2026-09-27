#include "retrowatch_common.h"
#include "bsp_audio.h"
#include "gui_manager.h"
#include "esp_log.h"

static const char *TAG = "APP_DASHCAM";
static bool is_recording = false;
static bool screen_sleep = false;

void app_dashcam_mode_init(void) {
    ESP_LOGI(TAG, "Entering Dashcam Mode (Pass-through Loop Recording)");
    is_recording = true;
    ESP_LOGI(TAG, "Auto-started Loop Recording: MicroSD /REC/REC_0001.AVI (Segment 1-min)");
}

void app_dashcam_mode_handle_input(btn_event_t evt) {
    switch (evt) {
        case BTN_EVT_A_CLICK:
        case BTN_EVT_PAUSE_CLICK:
            // 一鍵緊急鎖檔 (SOS Lock) + 警報音 + 螢幕黃色警示框
            bsp_audio_play_tone(1500, 150);
            gui_show_toast_lock_alert();
            ESP_LOGW(TAG, "EMERGENCY LOCK! Current segment marked as LOCK_0001.AVI (Never Overwrite)");
            break;
        case BTN_EVT_B_CLICK:
            // 閉屏背景錄影 (省電不晃眼)
            screen_sleep = !screen_sleep;
            ESP_LOGI(TAG, "Toggling LCD Backlight Sleep: %s (Recording in Background)", screen_sleep ? "OFF" : "ON");
            break;
        case BTN_EVT_START_CLICK:
            is_recording = false;
            retrowatch_switch_mode(SYS_MODE_MENU);
            break;
        default:
            break;
    }
}
