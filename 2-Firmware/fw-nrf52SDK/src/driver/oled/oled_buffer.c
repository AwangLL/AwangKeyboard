#include "oled_api.h"
#include <string.h>
#include <stdint.h>
//#include "nrf_delay.h"

/************

      0			    127
     0+---------------------+
      |			    		|
      |			    		|
      |			    		|
      |			   	 		|
      |			    		|
    63+---------------------+



************/

uint8_t oled_display_buffer[OLED_SIZE_LENGTH * OLED_SIZE_PAGE];

/**@brief 向buffer指定页中写入5x8字符
 *
 * @param[in] row     行
 * @param[in] offset  列
 * @param[in] *str    字符串
 * @param[in] align   文本对齐方式
 */
void oled_buffer_draw_font5x8(uint8_t row, uint8_t col, char *str, enum text_align align)
{
	if (row >= OLED_SIZE_WIDTH || col >= OLED_SIZE_LENGTH)
		return;

	uint8_t str_len = strlen(str);
	uint8_t total_len = str_len * 6;
	uint8_t p, i, j;
	int8_t c;
	uint64_t temp_buf;

	switch (align)
	{
	case TEXT_ALIGN_LEFT:
		c = col;
		break;
	case TEXT_ALIGN_CENTER:
		c = (OLED_SIZE_LENGTH - total_len) / 2;
		break;
	case TEXT_ALIGN_RIGHT:
		c = col - total_len;
		break;
	default:
		return;
	}
	if (c < 0)
		c = 0;
	p = row / 8;
	while (*str != '\0')
	{
		for (i = 0; i < 5; i++)
		{
			temp_buf = 0;
			//获取原buf
			for (j = 0; j < OLED_SIZE_PAGE; j++)
			{
				temp_buf |= (uint64_t)oled_display_buffer[j * OLED_SIZE_LENGTH + c] << (8 * j);
			}
			//指定位置零
			temp_buf &= ~((uint64_t)0xff << row);
			//指定位操作
			temp_buf |= (uint64_t)oled_font5x8[(*str - 0x20) * 5 + i] << row;

			oled_display_buffer[p * 128 + c] = temp_buf >> (8 * p);
			if (p + 1 < OLED_SIZE_PAGE)
				oled_display_buffer[(p + 1) * 128 + c] |= temp_buf >> (8 * (p + 1));
			c++;
		}
		temp_buf = 0;
		//获取原buf
		for (j = 0; j < OLED_SIZE_PAGE; j++)
		{
			temp_buf |= (uint64_t)oled_display_buffer[j * OLED_SIZE_LENGTH + c] << (8 * j);
		}
		//指定位置零
		temp_buf &= ~((uint64_t)0xff << row);

		oled_display_buffer[p * OLED_SIZE_LENGTH + c] = temp_buf >> (8 * p);
		if (p + 1 < OLED_SIZE_PAGE)
			oled_display_buffer[(p + 1) * OLED_SIZE_LENGTH + c] = temp_buf >> (8 * (p + 1));
		c++;
		str++;
	}
}

void oled_buffer_draw_font5x8_inverse(uint8_t row, uint8_t col, char *str, enum text_align align)
{
	if (row >= OLED_SIZE_WIDTH || col >= OLED_SIZE_LENGTH)
		return;

	uint8_t str_len = strlen(str);
	uint8_t total_len = str_len * 6;
	uint8_t p, i, j;
	int8_t c;
	uint64_t temp_buf;

	switch (align)
	{
	case TEXT_ALIGN_LEFT:
		c = col;
		break;
	case TEXT_ALIGN_CENTER:
		c = (OLED_SIZE_LENGTH - total_len) / 2;
		break;
	case TEXT_ALIGN_RIGHT:
		c = col - total_len;
		break;
	default:
		return;
	}
	if (c < 0)
		c = 0;
	p = row / 8;
	while (*str != '\0')
	{
		for (i = 0; i < 5; i++)
		{
			temp_buf = 0;
			//获取原buf
			for (j = 0; j < OLED_SIZE_PAGE; j++)
			{
				temp_buf |= (uint64_t)oled_display_buffer[j * OLED_SIZE_LENGTH + c] << (8 * j);
			}
			//指定位置零
			temp_buf &= ~((uint64_t)0xff << row);
			//指定位操作
			temp_buf |= ((uint64_t)((uint8_t)~oled_font5x8[(*str - 0x20) * 5 + i]) << row);

			oled_display_buffer[p * 128 + c] = temp_buf >> (8 * p);
			if (p + 1 < OLED_SIZE_PAGE)
				oled_display_buffer[(p + 1) * 128 + c] |= temp_buf >> (8 * (p + 1));
			c++;
		}
		temp_buf = 0;
		//获取原buf
		for (j = 0; j < OLED_SIZE_PAGE; j++)
		{
			temp_buf |= (uint64_t)oled_display_buffer[j * OLED_SIZE_LENGTH + c] << (8 * j);
		}
		//指定位置1
		temp_buf |= (uint64_t)0xff << row;

		oled_display_buffer[p * OLED_SIZE_LENGTH + c] = temp_buf >> (8 * p);
		if (p + 1 < OLED_SIZE_PAGE)
			oled_display_buffer[(p + 1) * OLED_SIZE_LENGTH + c] = temp_buf >> (8 * (p + 1));
		c++;
		str++;
	}
}

/**@brief在指定位置画一个图标
 *
 * @param[in] page
 * @param[in] col
 * @param[in] icon
 */
void oled_buffer_draw_picture(uint8_t row, uint8_t col, const pic_info_t *pic, enum text_align align)
{
	if (row >= OLED_SIZE_WIDTH || col >= OLED_SIZE_LENGTH)
		return;

	uint8_t i, j, p;
	int16_t c;
	uint64_t temp_buf;
	uint64_t pic_buf;
	switch (align)
	{
	case TEXT_ALIGN_LEFT:
		c = col;
		j = 0;
		break;
	case TEXT_ALIGN_RIGHT:
		c = col - pic->width;
		if (c < 0)
		{
			j = pic->length - col;
			c = 0;
		}
		else
			j = 0;
		break;
	default:
		return;
	}
	for (; j < pic->length && c < OLED_SIZE_LENGTH; c++, j++)
	{
		//图片缓存区
		pic_buf = 0;
		for (p = 0; p < pic->width / 8; p++)
		{
			pic_buf |= (uint64_t)pic->data[p * pic->length + j] << (8 * p);
		}
		//临时缓存区
		temp_buf = 0;
		for (p = 0; p < OLED_SIZE_PAGE; p++)
		{
			temp_buf |= (uint64_t)oled_display_buffer[p * OLED_SIZE_LENGTH + c] << (8 * p);
		}
		//指定位置零
		for (i = row; i < row + pic->width; i++)
		{
			temp_buf &= ~((uint64_t)0x1 << i);
		}
		//合并图片缓存与临时缓存
		temp_buf |= pic_buf << row;
		for (p = 0; p < OLED_SIZE_PAGE; p++)
		{
			oled_display_buffer[p * OLED_SIZE_LENGTH + c] = temp_buf >> (8 * p);
		}
	}
}

/**@brief在指定位置画一个图标
 *
 * @param[in] page
 * @param[in] col
 * @param[in] icon
 */
void oled_buffer_draw_icon(uint8_t page, enum text_align align, uint8_t col, const pic_info_t *icon)
{
	if (col >= OLED_SIZE_LENGTH || page >= OLED_SIZE_PAGE)
		return;
	uint8_t bit = ((icon->width - 1) / 8) + 1;
	int16_t i, j, c, p;
	switch (align)
	{
	case TEXT_ALIGN_LEFT:
		for (c = col, i = 0; c < OLED_SIZE_LENGTH && i < icon->length; i++, c++)
		{
			for (p = page, j = 0; p < OLED_SIZE_PAGE && j < bit; j++, p++)
			{
				oled_display_buffer[p * OLED_SIZE_LENGTH + c] = icon->data[j * icon->length + i];
			}
		}
		break;
	case TEXT_ALIGN_RIGHT:
		for (c = col, i = icon->length - 1; c >= 0 && i >= 0; i--, c--)
		{
			for (p = page, j = 0; p < OLED_SIZE_PAGE && j < bit; j++, p++)
			{
				oled_display_buffer[p * OLED_SIZE_LENGTH + c] = icon->data[j * icon->length + i];
			}
		}
		break;
	default:
		break;
	}
}

void oled_buffer_show_area(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY)
{
	oled_i2c_start();
	oled_i2c_stop();
}

void oled_buffer_show_page(uint8_t page, uint8_t startC, uint8_t endC)
{
	oled_i2c_start();
	oled_show_data(page, startC, endC - startC + 1, &oled_display_buffer[page * OLED_SIZE_LENGTH + startC]);
	oled_i2c_stop();
}
void oled_buffer_show_all(void)
{
	oled_i2c_start();
	for (uint8_t p = 0; p < OLED_SIZE_PAGE; p++)
	{
		oled_show_data(p, 0, OLED_SIZE_LENGTH, &oled_display_buffer[p * OLED_SIZE_LENGTH]);
	}
	oled_i2c_stop();
}
void oled_buffer_clear(void)
{
	memset(oled_display_buffer, 0x00, sizeof(oled_display_buffer));
}