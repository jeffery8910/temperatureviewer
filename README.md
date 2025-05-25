# Temperature Viewer 溫濕度顯示與指示燈專案

## 專案簡介
本專案使用 ESP32 開發板，結合 AHT20 感測器、TFT 顯示器與 RGB LED，定時讀取環境溫濕度並顯示於螢幕上，同時以 LED 顏色提示濕度範圍。按下按鈕可切換攝氏與華氏顯示單位。

## 硬體清單
- ESP32 開發板
- Adafruit AHT20 溫濕度感測器模組
- TFT 顯示模組（使用 TFT_eSPI 驅動）
- RGB LED（三色共陽或共陰皆可）
- 按鈕模組（連接至內部 pull-up 腳位）
- 連接線若干

## 腳位配置
```text
ESP32    AHT20    TFT      RGB LED       按鈕
-----    -----    ---      -------       ----
SDA(21)  SDA      -        -             -
SCL(22)  SCL      -        -             -
25       -        -        LED_R (PWM)   -
26       -        -        LED_G (PWM)   -
27       -        -        LED_B (PWM)   -
0        -        -        -             BUTTON

TFT 接腳請依 TFT_eSPI 庫設定（`User_Setup.h`）。
```  

## 軟體依賴
- Arduino IDE（1.8.x 或更新）
- [Adafruit_AHTX0](https://github.com/adafruit/Adafruit_AHTX0)
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [Button2](https://github.com/LennartHennigs/Button2)

請在 Arduino IDE 的「管理函式庫」中安裝上述套件。

## 程式結構
- `temperatureviewer.ino`：主程式，包含初始化、感測讀取與畫面更新邏輯。

主要流程：
1. 初始化 TFT、I2C、LED PWM 與按鈕中斷。
2. 每 5 秒讀取一次溫濕度感測值。
3. 顯示溫濕度資料，並根據濕度決定 LED 顏色：
   - 濕度 < 70%：藍色
   - 70% ≤ 濕度 ≤ 80%：綠色
   - 濕度 > 80%：紅色
4. 按下按鈕可在攝氏（°C）與華氏（°F）顯示間切換。

## 使用說明
1. 開啟 `temperatureviewer.ino` 並確認已選擇 ESP32 板與正確的通訊埠。
2. 如有更動腳位，可在程式開頭定義區修改。
3. 上傳程式至開發板。
4. 通電後螢幕將顯示溫濕度，LED 顯示濕度狀態。
5. 按下按鈕切換顯示單位。

## 範例程式片段
```cpp
#include <Adafruit_AHTX0.h>
#include <TFT_eSPI.h>
#include <Button2.h>

// ...existing code...

aht.getEvent(&humidity, &temp);
float tC  = temp.temperature;
float tDisp = useCelsius ? tC : tC * 1.8 + 32;
char unit = useCelsius ? 'C' : 'F';
// 顯示與 LED 控制
// ...existing code...
```

## 授權
本專案採用 MIT License，歡迎自由使用與修改。
