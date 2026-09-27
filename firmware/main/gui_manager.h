#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "retrowatch_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// UI/UX 狀態與視覺反饋
typedef enum {
    UI_ANIM_NONE = 0,
    UI_ANIM_SLIDE_LEFT,
    UI_ANIM_SLIDE_RIGHT,
    UI_ANIM_FADE
} ui_anim_type_t;

// 初始化 LVGL 圖形與顯示緩衝
void gui_manager_init(void);

// 渲染五大模式的頂級復古 UI/UX 畫面與常駐狀態列
void gui_render_global_status_bar(void); // 頂部常駐狀態列 (顯示目前功能/電量/Wi-Fi/音效/時間)
void gui_render_watch_face(const char *time_str, const char *stock_name, float price, float change_pct);
void gui_render_camera_osd(cam_filter_t filter_type, uint32_t remaining_shots, bool show_grid);
void gui_render_dashcam_hud(bool is_recording, bool is_locked, uint32_t record_seconds);
void gui_render_ipcam_hud(const char *ip_addr, bool motion_detected, uint8_t client_count);
void gui_render_carousel_menu(sys_mode_t selected_item, float scroll_offset);

// 觸發視覺動效與 HUD 彈窗 (例如一鍵鎖檔黃色警示框、拍照閃白、低電量紅標)
void gui_trigger_flash_effect(void);
void gui_show_toast_lock_alert(void);
void gui_show_toast_snapshot(void);

#ifdef __cplusplus
}
#endif
