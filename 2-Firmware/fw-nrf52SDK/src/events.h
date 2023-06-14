#pragma once

enum kbd_evt_list
{
    /**
     * @brief 驱动初始化事件
     * @ref   NULL
     */
    KBD_EVT_INIT,
    /**
     * @brief 驱动开始事件
     * @ref   NULL
     */
    KBD_EVT_START,
    /**
     * @brief 蓝牙状态改变事件
     * @ref   enum ble_state
     */
    KBD_EVT_BLE,
    /**
     * @brief USB状态改变事件
     * @ref   enum usb_state
     */
    KBD_EVT_USB,
    /**
     * @brief 睡眠事件
     * @ref   NULL
     */
    KBD_EVT_SLEEP,
    /**
     * @brief 省电模式状态改变事件
     * @ref   enum powersave_mode
     */
    KBD_EVT_LIGHT,

    KBD_EVT_OLED,
    /**
     * @brief 旋钮动作
     * @ref   enum encoder_action
     */
    KBD_EVT_ENCODER,

    KBD_EVT_BATTUPDATE,
};

/**
 * @brief 蓝牙状态
 * 
 */
enum ble_state
{
    BLE_STATE_IDLE,       // 蓝牙处于空闲状态
    BLE_STATE_DISCONNECT, // 蓝牙连接断开
    BLE_STATE_CONNECTED,  // 蓝牙已连接
    BLE_STATE_FAST_ADV,   // 蓝牙快速广播状态
    BLE_STATE_SLOW_ADV,   // 蓝牙慢速广播状态
    BLE_STATE_STOP
};

/**
 * @brief USB状态
 *
 */
enum usb_state
{
    USB_STATE_NOT_CONNECT,   //USB 未连接
    USB_STATE_POWER_CONNECT, //USB 已连接电源
    USB_STATE_WORKING,       //USB 连接到主机且正在工作(启用USB HID)
};

/**
 * @brief 省电事件状态
 *
 */
enum powersave_mode
{
    PWR_SAVE_ON,    // 启用省电模式
    PWR_SAVE_OFF,   // 禁用省电模式
    PWR_SAVE_ENTER, // 进入省电模式
    PWR_SAVE_EXIT,  // 退出省电模式
};

/**
 * @brief HID协议状态
 *
 */
enum hid_protocol_type
{
    HID_BLE_PROTOCOL, // ble hid
    HID_USB_PROTOCOL, // usb hid
};

enum light_command
{
    LIGHT_MODE_PREV,    // 前一个灯效
    LIGHT_MODE_NEXT,    // 下一个灯效
    LIGHT_BRI_DEC,      // 亮度减弱
    LIGHT_BRI_INC,      // 亮度增加
    LIGHT_COLOR_PREV,   // 前一个颜色
    LIGHT_COLOR_NEXT,   // 后一个颜色
    LIGHT_SWITCH,       // 开关
};

enum encoder_action
{
    EC_KEY_PRESSED,  // 按下
    EC_KEY_RELEASED, // 抬起
    EC_ROTATE_LEFT,  // 左旋
    EC_ROTATE_RIGHT  // 右旋
};
