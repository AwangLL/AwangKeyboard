### Events

##### BLE

| USER_STATE_BLE         |                  |
| ---------------------- | ---------------- |
| STATE_BLE_IDLE         | 蓝牙处于空闲状态 |
| STATE_BLE_CONNECTED    | 蓝牙已连接       |
| STATE_BLE_DISCONNECTED | 蓝牙连接断开     |
| STATE_BLE_FAST_ADV     | 蓝牙快速广播模式 |
| STATE_BLE_SLOW_ADV     | 蓝牙慢速广播模式 |

##### BATTERY

| USER_STATE_BATTERY     |              |
| ---------------------- | ------------ |
| STATE_BATTERY_IDLE     | 电池未充电   |
| STATE_BATTERY_CHARGING | 电池正在充电 |
| STATE_BATTERY_DONE     | 电池充电完成 |

##### KEYBOARD

| USER_STATE_KEYBOARD     |                |
| ----------------------- | -------------- |
| STATE_KEYBOARD_UNINITED | 键盘未初始化   |
| STATE_KEYBOARD_INITING  | 键盘正在初始化 |
| STATE_KEYBOARD_INITED   | 键盘已初始化   |
| STATE_KEYBOARD_SLEEP    | 键盘睡眠       |

##### ENCODER

| USER_EVT_ENCODER         |                    |
| ------------------------ | ------------------ |
| EVT_ENCODER_STATE_CHANGE | 旋转编码器模式切换 |

| USER_STATE_ENCODER             |                   |
| ------------------------------ | ----------------- |
| STATE_ENCODER_VOLUMN           | 旋钮 调节媒体音量 |
| STATE_ENCODER_LIGHT_BRIGHTNESS | 旋钮 调节灯光亮度 |
| STATE_ENCODER_LIGHT_MODE       | 旋钮 调节灯光灯效 |
| STATE_ENCODER_LIGHT_COLOR      | 旋钮 调节灯光颜色 |

##### LIGHT

| USER_EVT_LIGHT              |                      |
| --------------------------- | -------------------- |
| EVT_LIGHT_BRITNESS_INCREASE | 灯光亮度 +           |
| EVT_LIGHT_BRITNESS_DECREASE | 灯光亮度 -           |
| EVT_LIGHT_MODE_PREV         | 切换至上一种灯光效果 |
| EVT_LIGHT_MODE_NEXT         | 切换至下一种灯光效果 |
| EVT_LIGHT_COLOR_PREV        | 切换至上一种颜色     |
| EVT_LIGHT_COLOR_NEXT        | 切换至下一种颜色     |

##### OLED

| USER_EVT_OLED       |           |
| ------------------- | --------- |
| EVT_OLED_SWITCH_OFF | 关闭 OLED |
| EVT_OLED_SWITCH_ON  | 开启 OLED |

| USER_STATE_OLED             |                   |
| --------------------------- | ----------------- |
| STATE_OLED_ON               | OLED 开           |
| STATE_OLED_OFF              | OLED 关           |
| STATE_OLED_MENU_START       | OLED 开始菜单     |
| STATE_OLED_MENU_MAIN        | OLED 主菜单       |
| STATE_OLED_MENU_ENCODER     | OLED 旋钮菜单     |
| STATE_OLED_MENU_LIGHT_MODE  | OLED 灯效菜单     |
| STATE_OLED_MENU_LIGHT_COLOR | OLED 灯光颜色菜单 |

