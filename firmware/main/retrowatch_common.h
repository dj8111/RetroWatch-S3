#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "esp_log.h"

#ifdef __cplusplus
extern "C" {
#endif

// ==============================================================================
// 1. 硬體 GPIO 引腳終極映射表 (避開 Octal PSRAM GPIO 33~37)
// ==============================================================================
// ST7789 2.0" IPS LCD (SPI2/FSPI)
#define PIN_LCD_MOSI        38
#define PIN_LCD_SCLK        39
#define PIN_LCD_CS          40
#define PIN_LCD_DC          41
#define PIN_LCD_RST         42
#define PIN_LCD_BL          45

// OV5640 5MP 相機 (DVP 24-Pin)
#define PIN_CAM_D0          11
#define PIN_CAM_D1          9
#define PIN_CAM_D2          8
#define PIN_CAM_D3          10
#define PIN_CAM_D4          12
#define PIN_CAM_D5          18
#define PIN_CAM_D6          17
#define PIN_CAM_D7          16
#define PIN_CAM_XCLK        15
#define PIN_CAM_PCLK        13
#define PIN_CAM_VSYNC       6
#define PIN_CAM_HREF        7
#define PIN_CAM_SIOD        47
#define PIN_CAM_SIOC        48

// MicroSD (TF) 卡槽
#define PIN_SD_CLK          43
#define PIN_SD_CMD          44
#define PIN_SD_D0           20

// 使用者按鍵輸入
#define PIN_NAV_ADC         1   // 五向導航 (ADC1_CH0 梯形分壓)
#define PIN_BTN_A           4   // A 鍵 / 拍照快門
#define PIN_BTN_B           5   // B 鍵 / 返回
#define PIN_BTN_START       21  // START 鍵
#define PIN_BTN_PAUSE       14  // PAUSE 鍵 / Wi-Fi AP 傳圖長按

// 蜂鳴器與電源管理
#define PIN_BUZZER_PWM      2   // S8050 NPN 蜂鳴器驅動
#define PIN_BATT_ADC        3   // 電池電壓 1/2 分壓 (ADC1_CH2)
#define PIN_VBUS_DET        46  // Type-C 5V 插入偵測 (高電位=插電)

// ==============================================================================
// 2. 系統模式列舉 (五合一架構)
// ==============================================================================
typedef enum {
    SYS_MODE_WATCH = 0,     // 桌面動態看盤翻頁時鐘
    SYS_MODE_CAMERA,        // 復古街拍微單 (25fps 取景 + 5MP 拍照)
    SYS_MODE_DASHCAM,       // 微型行車記錄器 (循環錄影 + 通電自錄 + 緊急鎖檔)
    SYS_MODE_IPCAM,         // 智慧居家遠端監控 (HTTP MJPEG 串流 + 移動偵測)
    SYS_MODE_GAME,          // 懷舊復古掌機 (Retro-Go 核心)
    SYS_MODE_MENU,          // 全域主選單
    SYS_MODE_MAX
} sys_mode_t;

// ==============================================================================
// 3. 按鍵事件列舉
// ==============================================================================
typedef enum {
    BTN_EVT_NONE = 0,
    BTN_EVT_UP,
    BTN_EVT_DOWN,
    BTN_EVT_LEFT,
    BTN_EVT_RIGHT,
    BTN_EVT_CENTER,
    BTN_EVT_A_CLICK,
    BTN_EVT_B_CLICK,
    BTN_EVT_START_CLICK,
    BTN_EVT_PAUSE_CLICK,
    BTN_EVT_PAUSE_LONG_PRESS
} btn_event_t;

// 相機濾鏡風格 (共 5 種，首項必為原始，以 GR 為主 + 富士相機風格)
typedef enum {
    CAM_FILTER_ORIGINAL = 0,        // 1. 原始 (Original / Standard): 自然純淨，真實還原現場色彩與光影
    CAM_FILTER_GR_HARD_BW,          // 2. GR 高反差黑白 (GR Hard B&W): 理光靈魂街拍，深邃純黑與極致銳利高光，強烈粗顆粒底片紀實感
    CAM_FILTER_GR_POSITIVE,         // 3. GR 經典正片 (GR Positive Film): 理光招牌街拍膠卷，飽和濃郁、青翠陰影、紅潤微暖、文青隨拍首選
    CAM_FILTER_FUJI_CHROME,         // 4. 富士經典正片 (Fuji Classic Chrome): 富士傳奇冷調紀實，微降飽和、藍青暗部、電影故事感
    CAM_FILTER_FUJI_CLASSIC_NEG,    // 5. 富士經典負片 (Fuji Classic Negative): 重現 Superia 底片懷舊質感，硬朗高對比、洋紅高光、深綠暗部
    CAM_FILTER_MAX
} cam_filter_t;

// 五大模組可透過手機 Web 配置之參數
typedef struct {
    char stock_symbols[4][8];       // 看盤自訂 4 檔代號 (例: AAPL, NVDA, BTC, 2330)
    uint8_t dashcam_segment_min;    // 行車記錄器分段 (1, 3, 5 分鐘)
    uint8_t ipcam_sensitivity;      // 監控移動偵測靈敏度 (1~10)
    char alarm_webhook_url[64];     // Line Notify 或 Telegram Webhook
    uint8_t screen_brightness;      // 螢幕亮度 (10~100%)
    uint8_t game_volume;            // 遊戲音量 (0~100)
    cam_filter_t default_cam_filter;// 預設相機濾鏡 (0~4)
} retrowatch_web_config_t;

// 全域共享狀態與使用者設定
typedef struct {
    sys_mode_t current_mode;
    bool is_usb_powered;
    uint8_t battery_percent;
    bool is_wifi_connected;
    char current_ip[16];
    
    // 相機專屬功能與狀態
    cam_filter_t current_filter;    // 當前作用中相機濾鏡 (0~4)
    bool sound_enabled;             // 相機快門與遊戲音效開關 (true: 開啟, false: 靜音)
    bool date_stamp_enabled;        // 拍照 YYYY-MM-DD 復古日期戳記開關
    uint8_t self_timer_sec;         // 倒數自拍 (0: 即時拍攝, 3: 3秒倒數自拍)
    bool timelapse_enabled;         // 縮時攝影開關
    uint16_t timelapse_interval_sec;// 縮時攝影拍攝間隔 (3 ~ 300 秒)
    uint32_t estimated_shots_left;  // MicroSD 剩餘預計可拍張數
    uint32_t estimated_time_min;    // 電池剩餘預計可用拍攝時間 (分鐘)
    
    // 系統電源與休眠狀態
    bool screen_is_awake;           // 螢幕是否點亮 (true: 亮屏, false: 30秒閒置休眠閉屏)
    int64_t last_activity_time;     // 最後一次按鍵活動時間戳記 (微秒)

    // 手機端同步配置
    retrowatch_web_config_t web_cfg;
} retrowatch_state_t;

extern retrowatch_state_t g_system_state;

// 模式切換與螢幕休眠喚醒函式
void retrowatch_switch_mode(sys_mode_t target_mode);
void retrowatch_feed_activity(void);   // 刷新活動時間 (延長亮屏)
void retrowatch_sleep_screen(void);    // 關閉螢幕背光 (30秒省電休眠)
void retrowatch_wake_screen(void);     // 喚醒螢幕背光 (點選任一鍵)

#ifdef __cplusplus
}
#endif
