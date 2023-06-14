#include "oled_api.h"
#include "oled_pic.h"

#include <string.h>
#include "nrfx_twi.h"
#include "nrf_delay.h"

#include "config.h"
#include "kb_nrf_print.h"

#define OLED_SLAVE_ADDRESS 0x3C
#define MAX_WRITE_LENGTH 128

static const nrfx_twi_t m_oled_i2c = NRFX_TWI_INSTANCE(0); //使用IIC0

const uint8_t oled_init_command[] = {
    0xAE, //display off
    0x20, //Set Memory Addressing Mode
    0x10, //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    0xb0, //Set Page Start Address for Page Addressing Mode,0-7
    0xc8, //Set COM Output Scan Direction
    0x00, //---set low column address
    0x10, //---set high column address
    0x40, //--set start line address
    0x81, //--set contrast control register
    0xff, //亮度调节 0x00~0xff
    0xa1, //--set segment re-map 0 to 127
    0xa6, //--set normal display
    0xa8, //--set multiplex ratio(1 to 64)
    0x3F, //
    0xa4, //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    0xd3, //-set display offset
    0x00, //-not offset
    0xd5, //--set display clock divide ratio/oscillator frequency
    0xf0, //--set divide ratio
    0xd9, //--set pre-charge period
    0x22, //
    0xda, //--set com pins hardware configuration
    0x12,
    0xdb, //--set vcomh
    0x20, //0x20,0.77xVcc
    0x8d, //--set DC-DC enable
    0x14, //
    0xaf  //--turn on oled panel
};

/**@brief TWI初始化
 */
void oled_twi_init(void)
{
    nrfx_twi_config_t p_config = {
        .scl = OLED_SCL_PIN,
        .sda = OLED_SDA_PIN,
        .frequency = NRF_TWI_FREQ_400K,
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
        //    .clear_bus_init     = false                   //初始化TWI时清除总线状态(恢复IIC为三态)
    };

    nrfx_twi_init(&m_oled_i2c, &p_config, NULL, NULL);
    nrfx_twi_enable(&m_oled_i2c);
}

/**@brief 向 i2c 写入数据
 *
 * @param[in] SlaveAddress  OLED的地址
 *            reg_addr      注册地址：0x00(命令) 和 0x40(数据)
 *            reg_data      注册数据
 *            len           数据长度
 *
 * @retrun err_code
 */
ret_code_t oled_i2c_write(uint8_t SlaveAddress, uint8_t reg_addr, uint8_t *reg_data, uint8_t len)
{
    ret_code_t err_code;

    uint8_t writedata[MAX_WRITE_LENGTH];

    if (len > MAX_WRITE_LENGTH + 1)
    {
        err_code = 1;
        return err_code;
    }

    writedata[0] = reg_addr;

    memcpy(&writedata[1], reg_data, len);

    err_code = nrfx_twi_tx(&m_oled_i2c, SlaveAddress, writedata, len + 1, false);
    kb_error(err_code);

    return err_code;
}
/**@brief 写入数据
 *
 * @buf 数据矩阵
 *
 * @len 数据长度
 */
void write_data(uint8_t *buf, uint8_t len)
{
    oled_i2c_write(OLED_SLAVE_ADDRESS, 0x40, buf, len);
}
/**@brief 写入命令
 *
 * @command 命令代码
 */
void write_cmd(uint8_t command)
{
    oled_i2c_write(OLED_SLAVE_ADDRESS, 0x00, &command, 1);
}

/**@brief 显示data
 *  
 * @param[in] page        数据所在的页数
 *            col_start   起始列
 *            len         数据长度
 */
void oled_show_data(uint8_t page, uint8_t col_start, uint8_t len, uint8_t *data)
{
    if (page >= OLED_SIZE_PAGE)
        return;

    if (col_start + len > OLED_SIZE_LENGTH)
        len = OLED_SIZE_LENGTH - col_start;

    write_cmd(0xb0 + page);
    write_cmd(0x00 + (col_start & 0xF));
    write_cmd(0x10 + ((col_start >> 4) & 0xF));

    write_data(data, len);
}

/**@brief 初始化OLED
 */
void oled_init(void)
{
    oled_twi_init();
    uint8_t i;
    uint8_t len = sizeof(oled_init_command) / sizeof(uint8_t);
    for (i = 0; i < len; i++)
        write_cmd(oled_init_command[i]);
    oled_i2c_stop();
}

void oled_switch_off(void)
{
    oled_i2c_start();
    write_cmd(0x8D);
    write_cmd(0x10);
    write_cmd(0xAE);
    oled_i2c_stop();
}
void oled_switch_on(void)
{
    oled_i2c_start();
    write_cmd(0x8D);
    write_cmd(0x14);
    write_cmd(0xAF);
    oled_i2c_stop();
}
void oled_i2c_start(void)
{
    nrfx_twi_enable(&m_oled_i2c);
    nrf_delay_us(20);
}
void oled_i2c_stop(void)
{
    nrfx_twi_disable(&m_oled_i2c);
}
