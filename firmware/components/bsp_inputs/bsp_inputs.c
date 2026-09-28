#include "bsp_inputs.h"
#include "driver/gpio.h"
#include "esp_timer.h"

static const char *TAG = "BSP_INPUTS";

// 初始化按鍵 GPIO (五向鍵 5 線個別接線 + 4 動作微動開關，全部啟用晶片內部上拉電阻)
esp_err_t bsp_inputs_init(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_NAV_UP) | (1ULL << PIN_NAV_DOWN) |
                        (1ULL << PIN_NAV_LEFT) | (1ULL << PIN_NAV_RIGHT) |
                        (1ULL << PIN_NAV_CENTER) | (1ULL << PIN_BTN_A) |
                        (1ULL << PIN_BTN_B) | (1ULL << PIN_BTN_START) |
                        (1ULL << PIN_BTN_PAUSE),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,     // 啟用內部上拉，放開為高電位 (1)，按下接地為低電位 (0)
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    ESP_LOGI(TAG, "Input system initialized: 5 Discrete Nav GPIOs (1,2,3,14,21) + 4 Action Buttons");
    return ESP_OK;
}

// 輪詢檢測按鍵事件 (個別數位 GPIO 讀取與消抖)
btn_event_t bsp_inputs_poll(void) {
    static int64_t pause_press_time = 0;
    static bool pause_is_pressed = false;

    // 1. 五向導航開關檢測 (個別數位 GPIO，按下時為 LOW)
    if (gpio_get_level(PIN_NAV_UP) == 0) {
        return BTN_EVT_UP;
    }
    if (gpio_get_level(PIN_NAV_DOWN) == 0) {
        return BTN_EVT_DOWN;
    }
    if (gpio_get_level(PIN_NAV_LEFT) == 0) {
        return BTN_EVT_LEFT;
    }
    if (gpio_get_level(PIN_NAV_RIGHT) == 0) {
        return BTN_EVT_RIGHT;
    }
    if (gpio_get_level(PIN_NAV_CENTER) == 0) {
        return BTN_EVT_CENTER;
    }

    // 2. 檢測 A 鍵 (兼拍照快門)
    if (gpio_get_level(PIN_BTN_A) == 0) {
        return BTN_EVT_A_CLICK;
    }

    // 3. 檢測 B 鍵 (返回)
    if (gpio_get_level(PIN_BTN_B) == 0) {
        return BTN_EVT_B_CLICK;
    }

    // 4. 檢測 START 鍵
    if (gpio_get_level(PIN_BTN_START) == 0) {
        return BTN_EVT_START_CLICK;
    }

    // 5. 檢測 PAUSE 鍵 (相容板載 BOOT 按鍵，支援長按 3 秒進入 AP 傳圖)
    if (gpio_get_level(PIN_BTN_PAUSE) == 0) {
        if (!pause_is_pressed) {
            pause_is_pressed = true;
            pause_press_time = esp_timer_get_time();
        } else {
            // 已按住超過 3,000,000 微秒 (3秒)
            if (esp_timer_get_time() - pause_press_time > 3000000) {
                pause_is_pressed = false;
                return BTN_EVT_PAUSE_LONG_PRESS;
            }
        }
    } else {
        if (pause_is_pressed) {
            pause_is_pressed = false;
            // 短按釋放
            if (esp_timer_get_time() - pause_press_time < 3000000) {
                return BTN_EVT_PAUSE_CLICK;
            }
        }
    }

    return BTN_EVT_NONE;
}
