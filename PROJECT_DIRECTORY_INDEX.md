# RetroWatch-S3 專案分門別類總覽與交付索引

本文件將 **RetroWatch-S3 微型復古多功能掌機** 的所有軟硬體資源、程式碼與 3D 列印發包檔案進行系統化分類索引，方便您依序推進。

---

## 📁 專案全景目錄樹結構

```
c:\Dev\微型復古多功能掌機\
├── 📘 BEGINNER_TUTORIAL_ZERO_TO_HERO.md   # 【新手必讀】零基礎保姆級全彩接線與組裝實戰全書
├── 📄 SPEC_AND_GUIDE.md                  # 【工程規格】硬體引腳、機構 Z 軸堆疊、CMF 與電路架構
├── 📄 PROJECT_PLAN_AND_OPERATION.md      # 【操作手冊】BOM 表、五大模式操作流程與選單互動樹
├── 📄 README.md                          # 專案主入口與核心特色導航
│
├── 📁 3d_models/                         # 【3D 列印與製造發包專用目錄】
│   ├── README_3D_PRINTING.md             # 🌟 提供給 3D 列印廠商的規格書、材質要求與發包話術
│   ├── generate_models.scad              # 🌟 OpenSCAD 參數化 3D 建模源代碼 (前殼/後蓋/按鍵帽)
│   └── faceplate_sticker_104x56.svg      # 🌟 1:1 前面板香檳金金屬銘板 / 彩色背膠貼紙印刷圖紙
│
└── 📁 firmware/                          # 【ESP32-S3 核心韌體工程 (ESP-IDF v5.1+)】
    ├── CMakeLists.txt                    # 根構建腳本
    ├── sdkconfig.defaults                # 16MB Flash, 8MB Octal PSRAM, 240MHz 核心配置
    ├── partitions.csv                    # 16MB 專屬快閃記憶體分區表
    ├── main/
    │   ├── CMakeLists.txt                # 主應用構建腳本
    │   ├── retrowatch_common.h           # 全域 GPIO 定義、五合一模式與按鍵事件列舉
    │   ├── main.c                        # 系統初始化、開機和弦音、FreeRTOS 主任務與狀態機
    │   ├── app_watch_mode.c              # 桌面動態看盤時鐘與天氣跳錶引擎
    │   ├── app_camera_mode.c             # 復古微單 (25fps 取景、5MP 快門拍照、Wi-Fi 傳圖)
    │   ├── app_dashcam_mode.c            # 微型行車記錄器 (循環錄影、時間水印、一鍵鎖檔)
    │   ├── app_ipcam_mode.c              # 智慧居家遠端監控 (HTTP MJPEG 串流、移動偵測警報)
    │   └── app_game_mode.c               # 懷舊遊戲機模擬器 (Retro-Go 核心移植適配)
    └── components/
        ├── bsp_inputs/                   # 單線 ADC 梯形阻壓解碼驅動 (1根線搞定五向導航)
        └── bsp_audio/                    # 無源蜂鳴器 LEDC PWM 驅動 (快門音、開機和弦、PSG方波)
```

---

## 🎯 分門別類核心交付清單

### 第一類：3D 列印要提供給廠商的資料包
* **發包說明文件**：[3d_models/README_3D_PRINTING.md](3d_models/README_3D_PRINTING.md)
  * 包含：發給 3D 列印廠商的「**一鍵複製溝通話術**」、材料選擇（工業級高韌性類 ABS 樹脂）、外觀顏色（半透明燻黑 Smoke Black）、0.05mm 層厚與內側打支撐要求。
* **3D 幾何源碼**：[3d_models/generate_models.scad](3d_models/generate_models.scad)
  * 前殼主體（帶銘板沉台、壓克力沉台、Type-C 側槽、卡榫槽）。
  * 後蓋主體（帶 **高出 1.0mm 鏡頭火山口防護環**、左側平推插舌、70° 桌架凹槽、右側吊飾孔）。
  * A24 酒紅色階梯鍵帽、膠囊功能鍵帽。
* **面板印刷與裁切圖紙**：[3d_models/faceplate_sticker_104x56.svg](3d_models/faceplate_sticker_104x56.svg)
  * 1:1 實物比例（102 × 54 mm），具備拉絲香檳金、橘紅邊框飾線、文字印刷與按鍵開孔沖壓線。

### 第二類：燒錄在 ESP32-S3 板子上的程式內容
* **工程根目錄**：[firmware/](firmware/)
* **硬體設定與分區**：
  * [sdkconfig.defaults](firmware/sdkconfig.defaults)：開啟 16MB Flash、8MB Octal PSRAM、CPU 240MHz 全速執行。
  * [partitions.csv](firmware/partitions.csv)：劃分 4.5MB 應用程式區與 3.5MB 內置遊戲 ROM 區。
* **系統主程式與五大應用模組**：
  * [main.c](firmware/main/main.c)：開機和弦音、FreeRTOS 主任務排程、Type-C 插拔全速/節能切換、五合一狀態機。
  * [app_watch_mode.c](firmware/main/app_watch_mode.c)：翻頁時鐘與 Yahoo 股價即時跳錶。
  * [app_camera_mode.c](firmware/main/app_camera_mode.c)：25fps 即時取景、A 鍵快門拍照（帶喀嚓聲）、長按 PAUSE 鍵 AP 傳圖。
  * [app_dashcam_mode.c](firmware/main/app_dashcam_mode.c)：通電自錄、循環分段 AVI 錄影、一鍵緊急鎖檔、閉屏省電。
  * [app_ipcam_mode.c](firmware/main/app_ipcam_mode.c)：HTTP MJPEG 影像串流、雙核邊緣移動偵測與警報。
  * [app_game_mode.c](firmware/main/app_game_mode.c)：Retro-Go 模擬器載入。
* **底層硬體驅動組件**：
  * [bsp_inputs.c](firmware/components/bsp_inputs/bsp_inputs.c)：五向單線 ADC 梯形阻壓網絡解碼與防抖。
  * [bsp_audio.c](firmware/components/bsp_audio/bsp_audio.c)：蜂鳴器 LEDC PWM 快門聲與開機和弦音。

### 第三類：小白零基礎自製實戰手冊
* 詳見：[BEGINNER_TUTORIAL_ZERO_TO_HERO.md](BEGINNER_TUTORIAL_ZERO_TO_HERO.md)
  * 零基礎採購清單、第一次焊接 3 秒心法、彩色列線防呆表、積木式組裝步驟、防冒煙三用電表檢查、免寫代碼一鍵燒錄工具說明。
