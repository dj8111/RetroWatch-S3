#include "retrowatch_common.h"
#include "bsp_audio.h"
#include "esp_log.h"

static const char *TAG = "APP_GAME";

void app_game_mode_init(void) {
    ESP_LOGI(TAG, "Entering Retro-Go Gaming Mode (Loading ROMs from MicroSD /ROMS/)");
    ESP_LOGI(TAG, "Available Emulators: NES (Famicom), Game Boy (DMG), Game Boy Color (GBC)");
}

void app_game_mode_handle_input(btn_event_t evt) {
    switch (evt) {
        case BTN_EVT_PAUSE_CLICK:
            // 調出 Retro-Go 存檔與音效選單 (Save State / Load State / Mute Toggle)
            bsp_audio_button_tick();
            ESP_LOGI(TAG, "Opening Retro-Go In-Game Menu (Save State / Load State / Volume)");
            break;
        case BTN_EVT_CENTER:
            // 長按/短按中鍵快速切換遊戲音效靜音
            bsp_audio_toggle_sound();
            ESP_LOGI(TAG, "Game 8-bit Audio: %s", 
                     g_system_state.sound_enabled ? "UNMUTED (BGM & SFX ON)" : "MUTED (Silent Mode)");
            break;
        case BTN_EVT_START_CLICK:
            retrowatch_switch_mode(SYS_MODE_MENU);
            break;
        default:
            break;
    }
}
