# 实验1：Arduino IDE 与 ESP32 环境配置

## 实验内容
本实验完成 Arduino IDE 安装、ESP32 开发板支持包配置、USB 驱动安装、以及上传测试程序。

## 完成步骤
1. 安装 Arduino IDE。
2. 打开 Arduino IDE 设置，在 "Additional Boards Manager URLs" 中添加：
   - https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   - https://dl.espressif.com/dl/package_esp32_index.json
3. 在开发板管理器中搜索并安装 "esp32 by Espressif Systems"。
4. 安装 ESP32 USB 驱动（如 CH340 驱动），确认设备管理器中能识别 COM 端口。
5. 在 Arduino IDE 中选择开发板：`ESP32 Dev Module`。
6. 上传测试程序，观察 ESP32 板载 LED 每秒闪烁，并打开串口监视器查看输出。

## 实验程序
- `lab01/lab1.ino`：ESP32 Arduino 示例程序，板载 LED 每秒闪烁，并在串口打印 `Hello ESP32!`。

## 实验结果
- 已完成 Arduino IDE 安装与配置。
- ESP32 开发板支持包已添加。
- USB 串口驱动安装完成。
- 测试程序上传成功，LED 闪烁正常。
- 实验照片已添加为 `lab01/result.png`（可替换为真实截图）。

> 该目录包含实验说明文件、ESP32 程序和实验结果图片占位文件。
