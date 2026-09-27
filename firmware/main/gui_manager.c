#include "gui_manager.h"
#include "esp_log.h"
#include <stdio.h>

static const char *TAG = "GUI_MANAGER";

void gui_manager_init(void) {
    ESP_LOGI(TAG, "GUI Manager initialized with Game & Watch Visual Engine");
    ESP_LOGI(TAG, "Display: 320x240 @ 80MHz SPI DMA Double-Buffering");
}

// 頂部常駐全域狀態列 (高度 18px，常態顯示於螢幕最頂部)
void gui_render_global_status_bar(void) {
    const char *mode_names[] = {
        "【看盤翻頁鐘】",
        "【復古街拍機】",
        "【微型行車記錄】",
        "【智慧居家監控】",
        "【懷舊復古遊戲】",
        "【系統主選單】"
    };
    const char *cur_name = (g_system_state.current_mode < SYS_MODE_MAX) ? 
                           mode_names[g_system_state.current_mode] : "【系統】";
                           
    char batt_str[16];
    if (g_system_state.is_usb_powered) {
        snprintf(batt_str, sizeof(batt_str), "⚡CHG");
    } else {
        snprintf(batt_str, sizeof(batt_str), "🔋%d%%", g_system_state.battery_percent);
    }
    
    const char *sound_str = g_system_state.sound_enabled ? "🔊" : "🔇";
    const char *wifi_str = g_system_state.is_wifi_connected ? "📶" : "x";

    ESP_LOGD(TAG, "[Status Bar] %s | %s | %s | %s | 💾TF | 23:59:59", 
             cur_name, batt_str, sound_str, wifi_str);
}

// 1. 桌面看盤時鐘：擬真翻頁鐘 + 紅綠 K 線 / 金融跳錶卡片
void gui_render_watch_face(const char *time_str, const char *stock_name, float price, float change_pct) {
    gui_render_global_status_bar(); // 常駐狀態列
    // 渲染復古段碼風格時間 + 股票即時行情卡片
    ESP_LOGD(TAG, "[UI] Watch Render: Time=%s | Stock=%s ($%.2f, %+.2f%%)", 
             time_str, stock_name, price, change_pct);
}

// 2. 復古微單取景 HUD：對焦四角框、5大風格濾鏡標籤、剩餘可拍張數
void gui_render_camera_osd(cam_filter_t filter_type, uint32_t remaining_shots, bool show_grid) {
    const char *filter_names[CAM_FILTER_MAX] = {
        "1.ORIGINAL (原始)",
        "2.GR HARD B&W (高反差黑白)",
        "3.GR POSITIVE (經典正片)",
        "4.FUJI CHROME (經典正片)",
        "5.FUJI C-NEG (經典負片)"
    };
    const char *cur_filter = (filter_type < CAM_FILTER_MAX) ? filter_names[filter_type] : "ORIGINAL";
    ESP_LOGD(TAG, "[UI] Camera OSD: Filter=[%s], Left=%u shots, Grid=%s",
             cur_filter, remaining_shots, show_grid ? "ON" : "OFF");
}

// 3. 行車記錄器 HUD：紅色閃爍 REC 錄影指示燈、錄影時長、黃色緊急鎖檔標誌
void gui_render_dashcam_hud(bool is_recording, bool is_locked, uint32_t record_seconds) {
    uint32_t min = record_seconds / 60;
    uint32_t sec = record_seconds % 60;
    ESP_LOGD(TAG, "[UI] Dashcam HUD: REC=%s [%02u:%02u] | Status=%s",
             is_recording ? "● REC" : "■ STOP", min, sec, is_locked ? "🔒 LOCKED" : "NORMAL");
}

// 4. 智慧居家監控 HUD：即時串流 IP 水印、移動偵測動態雷達掃描框
void gui_render_ipcam_hud(const char *ip_addr, bool motion_detected, uint8_t client_count) {
    ESP_LOGD(TAG, "[UI] IPCam HUD: Stream=http://%s:80/stream | Motion=%s | Viewers=%d",
             ip_addr, motion_detected ? "⚠️ MOTION DETECTED" : "CLEAR", client_count);
}

// 5. 五合一環形輪播主選單：擬真卡帶平滑橫向滑動
void gui_render_carousel_menu(sys_mode_t selected_item, float scroll_offset) {
    const char *mode_titles[] = {
        "1. 桌面看盤翻頁鐘",
        "2. 復古街拍微單相機",
        "3. 微型行車記錄器",
        "4. 智慧居家遠端監控",
        "5. 懷舊遊戲機 (Retro-Go)"
    };
    if (selected_item < SYS_MODE_MAX) {
        ESP_LOGD(TAG, "[UI] Carousel Menu Selected: %s (Offset: %.2f)", 
                 mode_titles[selected_item], scroll_offset);
    }
}

// 快門拍照白色全屏瞬間閃爍 (視覺回饋)
void gui_trigger_flash_effect(void) {
    ESP_LOGI(TAG, "[UI Animation] Trigger Shutter Flash Screen Effect");
}

// 一鍵緊急鎖檔彈出黃色 Toast 警示框
void gui_show_toast_lock_alert(void) {
    ESP_LOGW(TAG, "[UI Toast] >>> 🔒 EMERGENCY FILE LOCKED <<< (Saved to LOCK_xxxx.AVI)");
}

// 監控截圖彈出綠色提示
void gui_show_toast_snapshot(void) {
    ESP_LOGI(TAG, "[UI Toast] >>> 📷 SNAPSHOT SAVED TO TF CARD <<<");
}
