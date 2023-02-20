/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2013 Chuck McManis <cmcmanis@mcmanis.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * SPI Port example
 */

#include <stdint.h>
#include <math.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/adc.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "gfx.h"
#include <libopencm3/stm32/usart.h> 

/* Convert degrees to radians */
#define d2r(d) ((d) * 6.2831853 / 360.0)
uint8_t com_en;
uint16_t battery;
uint8_t batt_alarm;

uint16_t read_reg(int reg);
void write_reg(uint8_t reg, uint8_t value);
uint8_t read_xyz(int16_t vecs[3]);
void spi_setup(void);

void spi_setup(void)
{
    rcc_periph_clock_enable(RCC_SPI5);
	
    rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOF);

	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
    gpio_set(GPIOC, GPIO1);

    gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE,
		GPIO7 | GPIO8 | GPIO9);   
	gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);

    spi_set_master_mode(SPI5);
	spi_set_baudrate_prescaler(SPI5, SPI_CR1_BR_FPCLK_DIV_64);
	spi_set_clock_polarity_0(SPI5);
	spi_set_clock_phase_0(SPI5);
	spi_set_full_duplex_mode(SPI5);
	spi_set_unidirectional_mode(SPI5);
	spi_enable_software_slave_management(SPI5);
	spi_send_msb_first(SPI5);
	spi_set_nss_high(SPI5);
    SPI_I2SCFGR(SPI5) &= ~SPI_I2SCFGR_I2SMOD;
	spi_enable(SPI5);


	/* Enable GPIOG clock. */
	rcc_periph_clock_enable(RCC_GPIOG);

	/* Set GPIO13 (in GPIO port G) to 'output push-pull'. */
	gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13 | GPIO14);

}

static void button_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Set GPIO0 (in GPIO port A) to 'input open-drain'. */
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
	gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13 | GPIO14);
}

static void my_usart_print_int(uint32_t usart, int32_t value)
{
	int8_t i;
	int8_t nr_digits = 0;
	char buffer[25];

	if (value < 0) {
		usart_send_blocking(usart, '-');
		value = value * -1;
	}

	if (value == 0) {
		usart_send_blocking(usart, '0');
	}

	while (value > 0) {
		buffer[nr_digits++] = "0123456789"[value % 10];
		value /= 10;
	}

	for (i = nr_digits-1; i >= 0; i--) {
		usart_send_blocking(usart, buffer[i]);
	}

	usart_send_blocking(usart, '\r');
	usart_send_blocking(usart, '\n');
}

/*
 * Chart of the various SPI ports (1 - 6) and where their pins can be:
 *
 *	 NSS		  SCK			MISO		MOSI
 *	 --------------   -------------------   -------------   ---------------
 * SPI1  PA4, PA15	  PA5, PB3		PA6, PB4	PA7, PB5
 * SPI2  PB9, PB12, PI0   PB10, PB13, PD3, PI1  PB14, PC2, PI2  PB15, PC3, PI3
 * SPI3  PA15*, PA4*	  PB3*, PC10*		PB4*, PC11*	PB5*, PD6, PC12*
 * SPI4  PE4,PE11	  PE2, PE12		PE5, PE13	PE6, PE14
 * SPI5  PF6, PH5	  PF7, PH6		PF8		PF9, PF11, PH7
 * SPI6  PG8		  PG13			PG12		PG14
 *
 * Pin name with * is alternate function 6 otherwise use alternate function 5.
 *
 * MEMS uses SPI5 - SCK (PF7), MISO (PF8), MOSI (PF9),
 * MEMS CS* (PC1)  -- GPIO
 * MEMS INT1 = PA1, MEMS INT2 = PA2
 */

int print_decimal(int);

/*
 * int len = print_decimal(int value)
 *
 * Very simple routine to print an integer as a decimal
 * number on the console.
 */
int
print_decimal(int num)
{
	int		ndx = 0;
	char	buf[10];
	int		len = 0;
	char	is_signed = 0;
	
	if (com_en) {
		if (num < 0) {
			is_signed++;
			num = 0 - num;
		}
		buf[ndx++] = '\000';
		do {
			buf[ndx++] = (num % 10) + '0';
			num = num / 10;
		} while (num != 0);
		ndx--;
		if (is_signed != 0) {
			console_putc('-');
			len++;
		}
		while (buf[ndx] != '\000') {
			console_putc(buf[ndx--]);
			len++;
		}
		gpio_toggle(GPIOG, GPIO13);
	}
	else{
		len = 0;
		gpio_clear(GPIOG, GPIO13);
	}
	return len; /* number of characters printed */
}

static void adc_setup(void)
{
	rcc_periph_clock_enable(RCC_ADC1);
  	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);

	adc_power_off(ADC1);
  	adc_disable_scan_mode(ADC1);
  	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);

	adc_power_on(ADC1);
}

static uint16_t read_adc_naiive(uint8_t channel)
{
	uint8_t channel_array[16];
	channel_array[0] = channel;
	adc_set_regular_sequence(ADC1, 1, channel_array);
	adc_start_conversion_regular(ADC1);
	while (!adc_eoc(ADC1));
	uint16_t reg16 = adc_read_regular(ADC1);
	return reg16;
}

void adc_update(void){
	battery = read_adc_naiive(1)*9/4095;
}

char *axes[] = { "X: ", "Y: ", "Z: " };

#define GYR_RNW			(1 << 7) /* Write when zero  (ahorita en 1, read)*/  
#define GYR_MNS			(1 << 6) /* Multiple reads when 1 */
#define GYR_WHO_AM_I		0x0F
#define GYR_OUT_TEMP		0x26
#define GYR_STATUS_REG		0x27
#define GYR_CTRL_REG1		0x20
#define GYR_CTRL_REG1_PD	(1 << 3)
#define GYR_CTRL_REG1_XEN	(1 << 1)
#define GYR_CTRL_REG1_YEN	(1 << 0)
#define GYR_CTRL_REG1_ZEN	(1 << 2)
#define GYR_CTRL_REG1_BW_SHIFT	4
#define GYR_CTRL_REG4		0x23
#define GYR_CTRL_REG4_FS_SHIFT	4

#define GYR_OUT_X_L		0x28
#define GYR_OUT_X_H		0x29

#define GYR_OUT_Y_L		0x2A
#define GYR_OUT_Y_H		0x2B

#define GYR_OUT_Z_L		0x2C
#define GYR_OUT_Z_H		0x2D

#define L3GD20_SENSITIVITY_250DPS  (0.00875F)      // Roughly 22/256 for fixed point match
#define L3GD20_SENSITIVITY_500DPS  (0.0175F)       // Roughly 45/256
#define L3GD20_SENSITIVITY_2000DPS (0.070F)        // Roughly 18/256
#define L3GD20_DPS_TO_RADS         (0.017453293F)  // degress/s to rad/s multiplier

/*
 * This then is the actual bit of example. It initializes the
 * SPI port, and then shows a continuous display of values on
 * the console once you start it. Typing ^C will reset it.
 */
int main(void)
{
	int16_t vecs[3];
	int p1, p2, p3;

	clock_setup();
	console_setup(115200);
	com_en = 1;

	spi_setup();
	adc_setup();

    gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, GYR_CTRL_REG1); 
	spi_read(SPI5);
	spi_send(SPI5, GYR_CTRL_REG1_PD | GYR_CTRL_REG1_XEN |
			GYR_CTRL_REG1_YEN | GYR_CTRL_REG1_ZEN |
			(3 << GYR_CTRL_REG1_BW_SHIFT));
	spi_read(SPI5);
	gpio_set(GPIOC, GPIO1); 

    gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, GYR_CTRL_REG4);
	spi_read(SPI5);
	spi_send(SPI5, (1 << GYR_CTRL_REG4_FS_SHIFT));
	spi_read(SPI5);
	gpio_set(GPIOC, GPIO1);

    console_puts("X\tY\tZ\n");

	sdram_init();
	lcd_spi_init();
	console_puts("LCD Inicializada\n");
	console_puts("Checkeo de patrones, oprima cualquier tecla\n");
	msleep(2000);

/*	(void) console_getc(1); */
	gfx_init(lcd_draw_pixel, 240, 320);
	gfx_fillScreen(LCD_GREY);
	gfx_fillRoundRect(10, 10, 220, 220, 5, LCD_WHITE);
	gfx_drawRoundRect(10, 10, 220, 220, 5, LCD_RED);
	gfx_fillCircle(20, 250, 10, LCD_RED);
	gfx_fillCircle(120, 250, 10, LCD_GREEN);
	gfx_fillCircle(220, 250, 10, LCD_BLUE);
	gfx_setTextSize(2);
	gfx_setCursor(15, 25);
	gfx_puts("STM32F4-DISCO");
	gfx_setTextSize(1);
	gfx_setCursor(15, 55);
	gfx_puts("Laboratorio 4: STM32: GPIO, ");
	gfx_setCursor(15, 66);
	gfx_puts("ADC, comunicaciones, Iot");
	gfx_setCursor(15, 77);
	gfx_puts("");
	gfx_setCursor(15, 100);
	gfx_puts("Estudiantes:");
	gfx_setCursor(15, 120);
	gfx_puts("Osvaldo Espinoza - B22384");
	gfx_setCursor(15, 135);
	gfx_puts("Luis Soto - B77573");
	lcd_show_frame();
	console_puts("Now it has a bit of structured graphics.\n");
	console_puts("Press a key for some simple animation.\n");
	msleep(8000);
/*	(void) console_getc(1); */
	gfx_setTextColor(LCD_YELLOW, LCD_BLACK);
	gfx_setTextSize(3);
	p1 = 0;
	p2 = 45;
	p3 = 90;
    
	while (1) {
        uint8_t temp;
        uint8_t who;
        int16_t gyr_x;
        int16_t gyr_y;
        int16_t gyr_z;
		char int_to_str[7];
		char lcd_out[3];

		adc_update();

		if (gpio_get(GPIOA, GPIO0)) {
			if(com_en){
				com_en = 0;
			}
			else{
				com_en = 1;
			}
		}

		sprintf(lcd_out, "%s", "X:");
		sprintf(int_to_str, "%d", gyr_x);
		strcat(lcd_out, int_to_str);

		gfx_fillScreen(LCD_BLACK);
		gfx_setCursor(15, 36);
		gfx_puts(lcd_out);
	
		sprintf(lcd_out, "%s", "Y:");
		sprintf(int_to_str, "%d", gyr_y);
		strcat(lcd_out, int_to_str);

		gfx_setCursor(15, 90);
		gfx_puts(lcd_out);

		sprintf(lcd_out, "%s", "Z:");
		sprintf(int_to_str, "%d", gyr_z);
		strcat(lcd_out, int_to_str);

		gfx_setCursor(15, 144);
		gfx_puts(lcd_out);

		sprintf(int_to_str, "%d", battery);

		gfx_setCursor(15, 198);
		gfx_puts("Batt: ");

		gfx_setCursor(130, 232);
		gfx_puts(int_to_str);

		gfx_setCursor(160, 232);
		gfx_puts("V");
	
		lcd_show_frame();
		
		gpio_clear(GPIOC, GPIO1);             
		spi_send(SPI5, GYR_WHO_AM_I | 0x80);
		spi_read(SPI5); 
		spi_send(SPI5, 0);    
		who=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_STATUS_REG | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		temp=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_TEMP | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		temp=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);  

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_X_L | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_x=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_X_H | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_x|=spi_read(SPI5) << 8;
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Y_L | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_y=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Y_H | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_y|=spi_read(SPI5) << 8;
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Z_L | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_z=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Z_H | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_z|=spi_read(SPI5) << 8;
		gpio_set(GPIOC, GPIO1);

        gyr_x = gyr_x*L3GD20_SENSITIVITY_500DPS;
        gyr_y = gyr_y*L3GD20_SENSITIVITY_500DPS;
        gyr_z = gyr_z*L3GD20_SENSITIVITY_500DPS;

		if(battery <= 7){
			batt_alarm = 1;
			gpio_set(GPIOG, GPIO14);
		}
		else{
			batt_alarm = 0;
			gpio_clear(GPIOG, GPIO14);
		}

	    print_decimal(gyr_x); console_puts("\t");
        print_decimal(gyr_y); console_puts("\t");
        print_decimal(gyr_z); console_puts("\t");
		print_decimal(batt_alarm); console_puts("\t");
		print_decimal(battery); console_puts("\n");

		int i;
		for (i = 0; i < 80000; i++)    /* Wait a bit. */
			__asm__("nop");

		msleep(100);
	}

	return 0;
}
