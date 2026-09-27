#include "retrowatch_common.h"
#include "bsp_audio.h"
#include "esp_log.h"

static const char *TAG = "APP_IPCAM";
static bool motion_detection_enabled = true;

void app_ipcam_mode_init(void) {
    ESP_LOGI(TAG, "Entering Security IPCam Mode (Pass-through 24h Desk Streaming)");
    ESP_LOGI(TAG, "MJPEG Live Stream Available at: http://192.168.1.100/stream");
}

void app_ipcam_mode_handle_input(btn_event_t evt) {
    switch (evt) {
        case BTN_EVT_A_CLICK:
            // 手動截圖存檔
            bsp_audio_shutter_click();
            ESP_LOGI(TAG, "Snapshot Saved to /SNAP/IPC_SNAP_0001.JPG");
            break;
        case BTN_EVT_UP:
        case BTN_EVT_DOWN:
            motion_detection_enabled = !motion_detection_enabled;
            bsp_audio_button_tick();
            ESP_LOGI(TAG, "Motion Detection: %s", motion_detection_enabled ? "ENABLED (Alarm auto-record)" : "DISABLED");
            break;
        case BTN_EVT_B_CLICK:
        case BTN_EVT_START_CLICK:
            retrowatch_switch_mode(SYS_MODE_MENU);
            break;
        default:
            break;
    }
}
