#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "esp_log.h"

#ifdef __cplusplus
extern "C" {
#endif

// ==============================================================================
// 1. 硬體 GPIO 引腳終極映射表 (ESP32-S3-N16R8 CAM 開發板 + 2.4" LCD 專用)
// ==============================================================================
// 金逸晨 2.4" TFT LCD ST7789 (8-Pin 藍板 SPI 模組)
#define PIN_LCD_SCLK        41  // 硬體 SPI 時鐘 (SCL)
#define PIN_LCD_MOSI        42  // 硬體 SPI 數據 (SDA)
#define PIN_LCD_DC          45  // 數據 / 命令選擇 (DC)
#define PIN_LCD_RST         46  // 螢幕復位 (RES，可接 GPIO 46 或直接接主板 EN 腳)
#define PIN_LCD_CS          -1  // 片選 (CS 直接在螢幕藍板短接 GND，永久致能，省 1 根線)
#define PIN_LCD_BL          -1  // 背光 (BLK 直接在螢幕藍板短接 3.3V 全亮，省 1 根線)

// OV5640 5MP 相機 (ESP32-S3-CAM 板載 24-Pin FPC 翻蓋座，免焊直插)
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
#define PIN_CAM_SIOD        4   // SCCB SDA
#define PIN_CAM_SIOC        5   // SCCB SCL

// MicroSD (TF) 卡槽 (ESP32-S3-CAM 板載插槽，SDMMC 1-Bit 模式，免焊直插)
#define PIN_SD_CLK          38
#define PIN_SD_CMD          39
#define PIN_SD_D0           40

// 使用者按鍵輸入 (五向導航開關「個別接線」+ 內部上拉，按下接地，零分壓電阻)
#define PIN_NAV_UP          1   // 五向鍵 - 上 (UP)
#define PIN_NAV_DOWN        2   // 五向鍵 - 下 (DOWN)
#define PIN_NAV_LEFT        3   // 五向鍵 - 左 (LEFT)
#define PIN_NAV_RIGHT       14  // 五向鍵 - 右 (RIGHT)
#define PIN_NAV_CENTER      21  // 五向鍵 - 中 (PRESS/OK)

// 動作微動開關 (內部上拉，按下接地)
#define PIN_BTN_A           44  // A 鍵 / 拍照快門 (板載 RX 引腳)
#define PIN_BTN_B           47  // B 鍵 / 返回
#define PIN_BTN_START       48  // START 鍵 / 主選單
#define PIN_BTN_PAUSE       0   // PAUSE 鍵 / 長按 AP 傳圖 (相容板載 BOOT 按鍵或外接微動)

// 蜂鳴器 (9018 貼片無源壓電式蜂鳴器，LEDC PWM 直驅，免外接三極管)
#define PIN_BUZZER_PWM      43  // 蜂鳴器 PWM 輸出 (板載 TX 引腳)
#define PIN_BATT_ADC        -1  // 電池電壓監測 (可選)
#define PIN_VBUS_DET        -1  // USB 供電自動偵測

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
