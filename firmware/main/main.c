#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "retrowatch_common.h"
#include "bsp_inputs.h"
#include "bsp_audio.h"

static const char *TAG = "MAIN_SYSTEM";

// 全域共享狀態實體
retrowatch_state_t g_system_state = {
    .current_mode = SYS_MODE_WATCH,
    .is_usb_powered = false,
    .battery_percent = 100,
    .is_wifi_connected = false,
    .current_ip = "0.0.0.0",
    .current_filter = CAM_FILTER_ORIGINAL, // 預設第 1 項必定為原始 (Original / Standard)
    .sound_enabled = true,       // 預設開啟音效 (快門/遊戲方波)
    .date_stamp_enabled = true   // 預設開啟 YYYY-MM-DD 復古日期時間戳記
};

// 宣告各模式介面
extern void app_watch_mode_init(void);
extern void app_watch_mode_handle_input(btn_event_t evt);

extern void app_camera_mode_init(void);
extern void app_camera_mode_handle_input(btn_event_t evt);

extern void app_dashcam_mode_init(void);
extern void app_dashcam_mode_handle_input(btn_event_t evt);

extern void app_ipcam_mode_init(void);
extern void app_ipcam_mode_handle_input(btn_event_t evt);

extern void app_game_mode_init(void);
extern void app_game_mode_handle_input(btn_event_t evt);

// 刷新使用者活動時間
void retrowatch_feed_activity(void) {
    g_system_state.last_activity_time = esp_timer_get_time();
}

// 關閉螢幕背光 (30 秒閒置休眠省電)
void retrowatch_sleep_screen(void) {
    if (g_system_state.screen_is_awake) {
        gpio_set_level(PIN_LCD_BL, 0); // 關閉 LCD 背光
        g_system_state.screen_is_awake = false;
        ESP_LOGI(TAG, "Screen Sleep -> Inactive for 30s, backlight OFF to save battery");
    }
}

// 喚醒螢幕背光 (按任意鍵喚醒)
void retrowatch_wake_screen(void) {
    if (!g_system_state.screen_is_awake) {
        gpio_set_level(PIN_LCD_BL, 1); // 點亮 LCD 背光
        g_system_state.screen_is_awake = true;
        ESP_LOGI(TAG, "Screen Wake -> Key pressed, backlight ON");
    }
    retrowatch_feed_activity();
}

// 模式切換狀態機
void retrowatch_switch_mode(sys_mode_t target_mode) {
    if (target_mode == g_system_state.current_mode) return;

    bsp_audio_button_tick();
    retrowatch_feed_activity();
    g_system_state.current_mode = target_mode;

    switch (target_mode) {
        case SYS_MODE_WATCH:
            app_watch_mode_init();
            break;
        case SYS_MODE_CAMERA:
            app_camera_mode_init();
            break;
        case SYS_MODE_DASHCAM:
            app_dashcam_mode_init();
            break;
        case SYS_MODE_IPCAM:
            app_ipcam_mode_init();
            break;
        case SYS_MODE_GAME:
            app_game_mode_init();
            break;
        case SYS_MODE_MENU:
            ESP_LOGI(TAG, "Entering 5-in-1 Carousel Main Menu");
            break;
        default:
            break;
    }
}

// 主輸入處理與狀態機分發任務
static void task_main_loop(void *pvParameters) {
    ESP_LOGI(TAG, "Main Controller Loop Started on Core 1");
    retrowatch_wake_screen(); // 開機預設點亮螢幕

    while (1) {
        // 1. 偵測 Type-C 5V (GPIO 46) 插拔狀態
        bool usb_now = (gpio_get_level(PIN_VBUS_DET) == 1);
        if (usb_now != g_system_state.is_usb_powered) {
            g_system_state.is_usb_powered = usb_now;
            retrowatch_wake_screen(); // 插拔時均自動點亮螢幕
            if (usb_now) {
                bsp_audio_play_tone(1760, 60);
                ESP_LOGI(TAG, "Type-C 5V Connected -> High-speed Pass-through Mode Enabled (Screen Always-ON)");
                // 行車記錄器模式下插電自動開始錄影
                if (g_system_state.current_mode == SYS_MODE_DASHCAM) {
                    app_dashcam_mode_init();
                }
            } else {
                bsp_audio_play_tone(880, 60);
                ESP_LOGI(TAG, "Type-C 5V Disconnected -> Battery Power Profile Enabled");
            }
        }

        // 2. 檢測 30 秒閒置自動休眠 (電池模式下且非縮時/錄影中)
        if (g_system_state.screen_is_awake && !g_system_state.is_usb_powered) {
            if (!g_system_state.timelapse_enabled && g_system_state.current_mode != SYS_MODE_DASHCAM) {
                int64_t idle_time = esp_timer_get_time() - g_system_state.last_activity_time;
                if (idle_time > 30000000) { // 30,000,000 微秒 = 30 秒
                    retrowatch_sleep_screen();
                }
            }
        }

        // 3. 輪詢按鍵事件
        btn_event_t evt = bsp_inputs_poll();
        if (evt != BTN_EVT_NONE) {
            // 若當前處於休眠黑屏，第一下按鍵只負責喚醒螢幕 (防誤觸)
            if (!g_system_state.screen_is_awake) {
                retrowatch_wake_screen();
                bsp_audio_button_tick();
                vTaskDelay(pdMS_TO_TICKS(100));
                continue;
            }

            retrowatch_feed_activity(); // 有按鍵操作，重設 30 秒計時

            // 全域主選單處理
            if (g_system_state.current_mode == SYS_MODE_MENU) {
                if (evt == BTN_EVT_UP || evt == BTN_EVT_LEFT) {
                    bsp_audio_button_tick();
                    ESP_LOGI(TAG, "Menu Carousel Prev Item");
                } else if (evt == BTN_EVT_DOWN || evt == BTN_EVT_RIGHT) {
                    bsp_audio_button_tick();
                    ESP_LOGI(TAG, "Menu Carousel Next Item");
                } else if (evt == BTN_EVT_A_CLICK || evt == BTN_EVT_CENTER) {
                    // 確認進入預設模式
                    retrowatch_switch_mode(SYS_MODE_CAMERA);
                } else if (evt == BTN_EVT_B_CLICK) {
                    retrowatch_switch_mode(SYS_MODE_WATCH);
                }
            } else {
                // 分發至對應子模式
                switch (g_system_state.current_mode) {
                    case SYS_MODE_WATCH:
                        app_watch_mode_handle_input(evt);
                        break;
                    case SYS_MODE_CAMERA:
                        app_camera_mode_handle_input(evt);
                        break;
                    case SYS_MODE_DASHCAM:
                        app_dashcam_mode_handle_input(evt);
                        break;
                    case SYS_MODE_IPCAM:
                        app_ipcam_mode_handle_input(evt);
                        break;
                    case SYS_MODE_GAME:
                        app_game_mode_handle_input(evt);
                        break;
                    default:
                        break;
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(15)); // 15ms 掃描週期
    }
}

// 系統啟動入口
void app_main(void) {
    ESP_LOGI(TAG, "=================================================");
    ESP_LOGI(TAG, "    RetroWatch-S3 (ESP32-S3 Firmware v1.2)       ");
    ESP_LOGI(TAG, "    5-in-1 Ultra-Slim Vintage Smart Console      ");
    ESP_LOGI(TAG, "=================================================");

    // 1. 初始化 NVS 存儲
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 2. 初始化 VBUS 檢測引腳 (GPIO 46)
    gpio_config_t vbus_conf = {
        .pin_bit_mask = (1ULL << PIN_VBUS_DET),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&vbus_conf);

    // 3. 初始化硬體輸入 (五向單線 ADC + 獨立微動)
    bsp_inputs_init();

    // 4. 初始化蜂鳴器音效
    bsp_audio_init();

    // 5. 播放開機復古 8-bit 和弦旋律
    bsp_audio_boot_melody();

    // 6. 預設進入桌面動態看盤時鐘模式
    app_watch_mode_init();

    // 7. 啟動主控制循環任務
    xTaskCreatePinnedToCore(task_main_loop, "main_loop_task", 4096, NULL, 5, NULL, 1);
}
