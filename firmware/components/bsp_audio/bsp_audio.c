#include "bsp_audio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "retrowatch_common.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          PIN_BUZZER_PWM
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_10_BIT // 10-bit 分辨率 (0~1023)
#define LEDC_DUTY_50            (512)             // 50% 佔空比方波

esp_err_t bsp_audio_init(void) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = 2000,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // 預設靜音
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    return ESP_OK;
}

// 切換音效開啟/關閉
bool bsp_audio_toggle_sound(void) {
    g_system_state.sound_enabled = !g_system_state.sound_enabled;
    return g_system_state.sound_enabled;
}

// 播放指定頻率的方波聲音 (若設定為靜音則不發聲)
void bsp_audio_play_tone(uint32_t freq_hz, uint32_t duration_ms) {
    if (!g_system_state.sound_enabled) {
        vTaskDelay(pdMS_TO_TICKS(duration_ms));
        return;
    }
    if (freq_hz == 0) {
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(duration_ms));
        return;
    }
    ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq_hz);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY_50);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    vTaskDelay(pdMS_TO_TICKS(duration_ms));
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

// 相機快門雙頻聲仿真 (4.0kHz 諧振高頻轉 2.0kHz 階躍音，發揮 9018 壓電蜂鳴器 65dB 最大聲壓)
void bsp_audio_shutter_click(void) {
    bsp_audio_play_tone(4000, 18);
    vTaskDelay(pdMS_TO_TICKS(5));
    bsp_audio_play_tone(2000, 25);
}

// 開機復古 8-bit 和弦音
void bsp_audio_boot_melody(void) {
    bsp_audio_play_tone(523, 70);  // C5
    bsp_audio_play_tone(659, 70);  // E5
    bsp_audio_play_tone(784, 70);  // G5
    bsp_audio_play_tone(1046, 120); // C6
}

// 按鍵微小點擊反饋音 (4000Hz 諧振微脈衝)
void bsp_audio_button_tick(void) {
    bsp_audio_play_tone(4000, 8);
}
