#pragma once
#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t bsp_audio_init(void);
void bsp_audio_play_tone(uint32_t freq_hz, uint32_t duration_ms);
void bsp_audio_shutter_click(void);
void bsp_audio_boot_melody(void);
void bsp_audio_button_tick(void);
bool bsp_audio_toggle_sound(void); // 切換音效開啟/關閉，返回當前狀態

#ifdef __cplusplus
}
#endif
