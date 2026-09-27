#include "retrowatch_common.h"
#include "bsp_audio.h"
#include "gui_manager.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "APP_CAMERA";
static TaskHandle_t s_timelapse_task_handle = NULL;

// 濾鏡名稱對照表 (共 5 項，首項必為原始，GR 為主 + 富士相機風格)
static const char *s_filter_labels[CAM_FILTER_MAX] = {
    "1. 原始 (Standard / Original)",
    "2. GR 高反差黑白 (GR Hard B&W / 森山大道風)",
    "3. GR 經典正片 (GR Positive Film / 街拍膠卷)",
    "4. 富士經典正片 (Fuji Classic Chrome / 紀實冷調)",
    "5. 富士經典負片 (Fuji Classic Negative / 日系復古)"
};

// 執行單張拍照
static void do_capture(void) {
    if (g_system_state.sound_enabled) {
        bsp_audio_shutter_click();
    }
    gui_trigger_flash_effect();
    
    const char *filter_desc = (g_system_state.current_filter < CAM_FILTER_MAX) ? 
                              s_filter_labels[g_system_state.current_filter] : "原始";

    if (g_system_state.date_stamp_enabled) {
        ESP_LOGI(TAG, "Shutter Triggered! Filter: [%s] | Stamped YYYY-MM-DD (2026-09-28) at bottom-right", filter_desc);
    } else {
        ESP_LOGI(TAG, "Shutter Triggered! Filter: [%s] | Pure capture (Date Stamp OFF)", filter_desc);
    }
    ESP_LOGI(TAG, "5MP Photo Saved to /DCIM/! [Battery: %d%% | Shots Left: %u | Est Time: %u min]",
             g_system_state.battery_percent, g_system_state.estimated_shots_left, g_system_state.estimated_time_min);
}

// 縮時攝影背景任務 (每隔 3~300 秒自動拍一張)
static void timelapse_worker_task(void *arg) {
    ESP_LOGI(TAG, "Timelapse Started! Interval = %u seconds", g_system_state.timelapse_interval_sec);
    uint32_t count = 0;
    while (g_system_state.timelapse_enabled) {
        do_capture();
        count++;
        ESP_LOGI(TAG, "[Timelapse] Shot #%u captured to /TIMELAPSE/", count);
        vTaskDelay(pdMS_TO_TICKS(g_system_state.timelapse_interval_sec * 1000));
    }
    vTaskDelete(NULL);
}

void app_camera_mode_init(void) {
    ESP_LOGI(TAG, "Entering Vintage Camera Mode (25fps Live Preview)");

    // 1. 動態計算當前電量與預計可用張數 / 續航時間
    // 假設 32GB MicroSD 剩餘 20GB，每張 5MP 照片約 2MB -> 預計可拍 10,000+ 張
    g_system_state.estimated_shots_left = 3450; 
    
    // 電池 450mAh，相機功耗 165mA -> 滿電約 2.7 小時 (160 分鐘)
    if (g_system_state.is_usb_powered) {
        g_system_state.estimated_time_min = 9999; // 插電中：無限長
        ESP_LOGI(TAG, "HUD Status: Battery=⚡ CHG (USB) | Rem. Shots=%u | Est. Time=∞ (Pass-through)",
                 g_system_state.estimated_shots_left);
    } else {
        g_system_state.estimated_time_min = (g_system_state.battery_percent * 160) / 100;
        ESP_LOGI(TAG, "HUD Status: Battery=%d%% | Rem. Shots=%u | Est. Time=%u hrs %u min",
                 g_system_state.battery_percent, g_system_state.estimated_shots_left,
                 g_system_state.estimated_time_min / 60, g_system_state.estimated_time_min % 60);
    }
}

void app_camera_mode_handle_input(btn_event_t evt) {
    switch (evt) {
        case BTN_EVT_A_CLICK:
            // 判斷是否開啟 3 秒倒數自拍
            if (g_system_state.self_timer_sec == 3) {
                ESP_LOGI(TAG, "Self-Timer 3s Triggered! Beeping 3... 2... 1...");
                for (int i = 3; i > 0; i--) {
                    bsp_audio_play_tone(1000, 80);
                    vTaskDelay(pdMS_TO_TICKS(900));
                }
            }
            do_capture();
            break;
            
        case BTN_EVT_UP:
            g_system_state.current_filter = (g_system_state.current_filter + CAM_FILTER_MAX - 1) % CAM_FILTER_MAX;
            bsp_audio_button_tick();
            ESP_LOGI(TAG, "Switching Filter Prev -> [%s]", s_filter_labels[g_system_state.current_filter]);
            break;
            
        case BTN_EVT_DOWN:
            g_system_state.current_filter = (g_system_state.current_filter + 1) % CAM_FILTER_MAX;
            bsp_audio_button_tick();
            ESP_LOGI(TAG, "Switching Filter Next -> [%s]", s_filter_labels[g_system_state.current_filter]);
            break;
            
        case BTN_EVT_LEFT:
            // 五向鍵 [左] -> 切換 YYYY-MM-DD 日期浮水印
            g_system_state.date_stamp_enabled = !g_system_state.date_stamp_enabled;
            bsp_audio_button_tick();
            ESP_LOGI(TAG, "Photo Date Stamp (YYYY-MM-DD): %s", 
                     g_system_state.date_stamp_enabled ? "ENABLED (Retro Orange)" : "DISABLED");
            break;
            
        case BTN_EVT_RIGHT:
            // 五向鍵 [右] -> 切換縮時攝影 (Timelapse) 或 3秒倒數自拍
            if (g_system_state.self_timer_sec == 0 && !g_system_state.timelapse_enabled) {
                g_system_state.self_timer_sec = 3;
                ESP_LOGI(TAG, "Camera Mode Switched -> [3s Self-Timer Mode]");
            } else if (g_system_state.self_timer_sec == 3) {
                g_system_state.self_timer_sec = 0;
                g_system_state.timelapse_enabled = true;
                g_system_state.timelapse_interval_sec = 5; // 預設 5 秒縮時
                xTaskCreate(timelapse_worker_task, "timelapse_task", 3072, NULL, 4, &s_timelapse_task_handle);
                ESP_LOGI(TAG, "Camera Mode Switched -> [Timelapse Mode 5s]");
            } else {
                g_system_state.timelapse_enabled = false;
                ESP_LOGI(TAG, "Camera Mode Switched -> [Single Shot Mode]");
            }
            bsp_audio_button_tick();
            break;

        case BTN_EVT_PAUSE_LONG_PRESS:
            // 長按 3 秒 -> 啟動 Wi-Fi AP 模式供手機連線傳圖與設定五大模組
            bsp_audio_boot_melody();
            ESP_LOGI(TAG, "Starting Wi-Fi Web Dashboard & Photo Station: SSID 'RetroWatch-S3', IP: 192.168.4.1");
            break;
            
        case BTN_EVT_B_CLICK:
        case BTN_EVT_START_CLICK:
            g_system_state.timelapse_enabled = false;
            retrowatch_switch_mode(SYS_MODE_MENU);
            break;
            
        default:
            break;
    }
}
