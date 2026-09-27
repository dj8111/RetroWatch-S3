#include "bsp_inputs.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_timer.h"

static const char *TAG = "BSP_INPUTS";
static adc_oneshot_unit_handle_t adc1_handle;

// 初始化按鍵 GPIO 與 ADC
esp_err_t bsp_inputs_init(void) {
    // 1. 初始化五向鍵單線 ADC (GPIO 1 / ADC1_CH0)
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12, // 支援 0~3.3V 全幅電壓
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &config));

    // 2. 初始化數位微動開關 (A, B, START, PAUSE)
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_BTN_A) | (1ULL << PIN_BTN_B) |
                        (1ULL << PIN_BTN_START) | (1ULL << PIN_BTN_PAUSE),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    ESP_LOGI(TAG, "Input system initialized (Single-Wire ADC Nav + 4 Discrete Buttons)");
    return ESP_OK;
}

// 輪詢檢測按鍵事件 (抗抖動演算法)
btn_event_t bsp_inputs_poll(void) {
    static int last_nav_adc = 4095;
    static int64_t pause_press_time = 0;
    static bool pause_is_pressed = false;

    // 1. 讀取單線 ADC 電壓 (12-bit, 0~4095)
    int adc_val = 0;
    adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &adc_val);

    // 梯形分壓電壓閥值判別 (3.3V 基準)
    // UP (0Ω) -> 0 ~ 300
    // DOWN (2.2k) -> 500 ~ 950
    // LEFT (5.1k) -> 1100 ~ 1550
    // RIGHT (12k) -> 1900 ~ 2400
    // CENTER (27k) -> 2650 ~ 3200
    if (adc_val < 3500) {
        // 有方向鍵被按下，且先前為釋放狀態 (防止連續重觸發)
        if (last_nav_adc > 3500) {
            last_nav_adc = adc_val;
            if (adc_val < 300) return BTN_EVT_UP;
            if (adc_val >= 500 && adc_val < 950) return BTN_EVT_DOWN;
            if (adc_val >= 1100 && adc_val < 1550) return BTN_EVT_LEFT;
            if (adc_val >= 1900 && adc_val < 2400) return BTN_EVT_RIGHT;
            if (adc_val >= 2650 && adc_val < 3200) return BTN_EVT_CENTER;
        }
    } else {
        last_nav_adc = 4095; // 釋放
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

    // 5. 檢測 PAUSE 鍵 (支援長按 3 秒進入 AP 傳圖)
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
