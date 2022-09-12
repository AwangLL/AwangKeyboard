# AwangKeyboard
> 基于nRF52840系列单片机，SDK采用nrf connect sdk v1.9.0，配合nrf connect sdk for vscode，硬件与外壳部分参考稚晖君的瀚文([HelloWord](https://github.com/peng-zhihui/HelloWord-Keyboard))机械键盘。

本仓库开源的资料包括
- 键盘主体与底座部分共5块PCB设计源文件，需使用立创专业版打开
- 外壳结构文件（对瀚文键盘的外壳做出部分改动）
- 键盘本体的固件源码（目前仅实现基础功能）

![[1.jpg]]

# 1. 项目说明

## 1.1 更新日志

TODO:
- [ ] 蓝牙
- [ ] 更多灯光效果
- [ ] 通信协议
- [ ] 上位机
- [ ] DFU
- [ ] 扩展模块

2022.9.12更新：
- 键盘主体的2块PCB以及底座的3块PCB设计文件
- 实现键盘基础功能的固件源码
- 外壳文件

## 1.2 项目文件

### 1.2.1 Hardware

Hardware文件是AwangKeyboard的硬件设计部分，包括键盘主体和底座部分的硬件源码以及立创开源工程[链接](https://oshwhub.com/lepis/awang-keyboard)。

目前一共有以下几块板子：
- 01-Main-Keyboard：键盘主控板，通过FPC排线与02连接。
- 02-Main-Connector：作为连接01与03的媒介，通过弹簧针与03连接。
- 03-Base-TypeC：用于连接电池、USB，以及进行锂电池充电管理。
- 04-Base-Hub1：用于连接扩展模块和05板。
- 05-Base-Hub2：板载两个Usb-A母座，作为UsbHub的两个外部接口，通过排针与04连接。

### 1.2.2 Firmware

Firmware文件提供了键盘主控板的固件源码。

- **Awang-Keyboard-fw**，键盘主控板固件，实现了按键扫描、RGB灯控制、USBHID以及USBCDC。

工程基于nrf connect sdk v1.9.0，配合nrf connect sdk for vscode插件，用vscode打开。

### 1.2.3 Layout

Layout文件提供键盘结构有关的所有文件，包括：

- 可供编辑的step外壳结构文件
- 可供编辑的定位板以及透明挡板dxf文件
- 可直接用于3d打印的stl文件
- 嘉立创自动生成的PCB 3d文件

### 1.2.4 Software

上位机（待开发）

### 1.2.5 Doc

参考资料、芯片的数据手册等

完整的键盘外壳需要用到**Layout/外壳**中的所有文件，四个stl文件可直接用于3d打印，dxf文件需进行相应的亚克力及PC的切割。（此为低成本方案，最终效果如效果图展示）

# 鸣谢

> [peng-zhihui/HelloWord-Keyboard (github.com)](https://github.com/peng-zhihui/HelloWord-Keyboard)

