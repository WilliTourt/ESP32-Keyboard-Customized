# ESP32 Keybrick

> Language: [English](readme.md) | [简体中文](readme-zh-CN.md)

一个基于 ESP32-C3 的多功能蓝牙键盘，名为 **ESP32 Keybrick**。设备提供 5 个可自定义的机械按键，可通过 BLE HID 向电脑或其他支持蓝牙键盘的设备发送按键、组合键和媒体控制指令。

本项目使用了 [Hid2Ble](https://github.com/BearLaboratory/Hid2Ble) 库，感谢该项目为 BLE HID 功能提供的帮助。

![ESP32 Keybrick 内部](https://github.com/user-attachments/assets/f8192b05-58b5-45bf-b381-eb0c5d716f49)

![ESP32 Keybrick 实物](https://github.com/user-attachments/assets/0e681c89-e31d-418d-b1d1-de1a4afd016d)

## 功能

- [√] 发送单个按键
- [√] 发送组合键
- [√] 发送媒体控制键
- [√] 倒计时
- [√] 节拍器
- [√] 在多个按键预设之间切换
- [√] 电池电量检测与低电量提示，BLE 连接状态指示
- [-] 不改动代码，直接在线创建新的按键预设
- [-] 自动重连

当前内置 7 个预设：

1. `Ctrl XCVZ`：Ctrl+X、Ctrl+C、Ctrl+V、Ctrl+Z、Ctrl+Shift+Z
2. `Win Combos`：Alt+Tab、Win+D、Ctrl+Alt+Del、Win+Shift+S、空置
3. `VSCode`：注释/解注释、Tab、Shift+Tab、快速打开、命令面板
4. `LCEDA Tools`：连线、过孔、铺铜、顶层铜、底层铜
5. `SW Drawing`：直线、圆、矩形、智能标注、法向
6. `Tabs`：5 个 Tab 键
7. `MediaCtrls`：亮度减小、亮度增加、音量减小、音量增加、播放/暂停

如需修改预设，请编辑 `ESP32-C3 BLE Keybrick/src/sys.cpp` 中的 `KeyPreset presets[PRESET_COUNT]`。目前需要修改代码后重新编译，不支持在线创建全新的预设。

编辑方式可参考已有的预设。HID 键值请参考网上关于 HID Usage Tables 的资料。

可以修改 `PRESET_COUNT` 来增减预设数量，当前预设数量为 7。

## 硬件

主要硬件：

- ESP32-C3-MINI-1 模组
- 5 个机械按键轴
- 1 个无源蜂鸣器
- 1 个状态指示灯
- 0.91 英寸 128×32 OLED 显示屏
- TP4056 锂电池充电模块
- 3.7 V 锂电池

引脚定义如下：

```cpp
#define ADC_PIN         A0
#define BUZZER_PIN      1
#define BTN_1_PIN       2
#define BTN_2_PIN       3
#define BTN_3_PIN       4
#define BTN_4_PIN       5
#define OLED_SDA        6
#define OLED_SCL        7
#define BTN_5_PIN       8
#define STATUS_LED      10
```

*按键输入当前在 `KEY_Init()` 中配置为 `INPUT`，因此需要外部上拉电阻。如果没有外部上拉电阻，请将 5 个按键的输入模式改为 `INPUT_PULLUP`，并根据实际接线确认按键电平逻辑。*

原理图与 PCB：

<img alt="原理图" src="https://github.com/user-attachments/assets/b504fdc1-eff4-463b-aabf-3dd12def60f8" />

![PCB 视图](https://github.com/user-attachments/assets/f91ffca2-36a5-414b-9cfa-0f3b87ed686d)

![PCB 视图](https://github.com/user-attachments/assets/62a9f4f8-30b5-4eb6-bd6f-50cd437f1e75)

## 编译与烧录

1. 安装 [PlatformIO](https://platformio.org/)（建议使用 VS Code 的 PlatformIO 插件）。
2. 在 PlatformIO 中打开 `ESP32-C3 BLE Keybrick` 文件夹。
3. 选择环境 `esp32-c3-devkitm-1`。
4. 编译并上传程序。

项目使用 Arduino 框架，具体配置见 `ESP32-C3 BLE Keybrick/platformio.ini`。

## 使用方法

首次启动时，默认预设为 `Ctrl XCVZ`：

| 按键 | 默认功能 |
| --- | --- |
| BTN1 | Ctrl+X |
| BTN2 | Ctrl+C |
| BTN3 | Ctrl+V |
| BTN4 | Ctrl+Z |
| BTN5 | Ctrl+Shift+Z |

设备通过 BLE 广播为 `ESP32C3 BLE Keybrick`，请在电脑或其他主机的蓝牙设置中搜索并连接它。

### 模式切换

- 长按 BTN4：进入节拍器模式。
- 长按 BTN5：进入倒计时设置模式；在倒计时或其他功能模式中长按 BTN5 可返回普通模式。
- 同时长按 BTN4 和 BTN5：进入预设选择模式。

### 倒计时设置

进入倒计时设置模式后：

- BTN1：调整小时
- BTN2：调整分钟
- BTN3：启用/开始倒计时
- BTN4：重置
- BTN5：返回普通模式

### 节拍器设置

进入节拍器模式后：

- BTN1：降低 BPM
- BTN2：提高 BPM
- BTN3：切换拍号
- BTN4：启动/停止节拍器

### 选择预设

同时长按 BTN4 和 BTN5 进入预设选择模式后：

- BTN1：选择上一个预设
- BTN2：选择下一个预设
- BTN3：确认并应用当前预设

确认后，当前预设会保存到 ESP32 的片上EEPROM中，重新上电后仍会使用该预设。

## 结构与资源

- `ESP32-C3 BLE Keybrick/`：PlatformIO 源代码文件夹
- `BOM_ESP32 Keybrick.xlsx`：物料清单
- `ESP32 BLE Keyboard v1.2f shell.stl`：外壳模型
- `ESP32 BLE Keyboard v1.2f cover.stl`：外壳盖板
- `ESP32-C3 BLE Keyboard v1.2.zip`：GERBER

## 装配注意事项

- 焊接 OLED 时不要将屏幕完全插到底，建议只插入焊盘长度的一部分，否则外壳与屏幕之间可能产生干涉。
- 机械按键轴和 OLED 焊接完成后，应剪短背面的引脚，避免凸起；背面还需要安装 TP4056 模块并容纳锂电池。
- 推荐使用 652272 规格的锂电池，厚度应不超过约 5.5 mm，以便放入外壳与 PCB 之间的空间。
- TP4056 模块需要直接焊接到 PCB 背面的焊盘区域。建议先焊接输入端的两个焊盘，再用镊子压住模块，焊接剩余四个焊盘。

## 已知问题

- ~~*An extra keystroke is triggered abnormally when returning to the main mode, sometimes*~~ (fixed)
- ~~*Return to main mode when metronome is enabled, keystroke is unavailable*~~ (fixed)

## 致谢

- 感谢 [BearLaboratory/Hid2Ble](https://github.com/BearLaboratory/Hid2Ble) 提供 BLE HID 库。
- 感谢 WoodBreeze 帮我做早期测试 :)

