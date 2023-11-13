#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "oled_display.h"
#include "fonts/fonts.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0

const uint8_t set_contrast = 0x81;
const uint8_t all_on = 0xA5;
const uint8_t display_resume = 0xA4;
const uint8_t normal_display = 0xA6;
const uint8_t inverted_display = 0xA7;
const uint8_t display_on = 0xAF;
const uint8_t display_off = 0xAE;

const uint8_t set_display_offset = 0xD3;
const uint8_t set_com_pins = 0xDA;

const uint8_t set_vcom_detect = 0xDB;

const uint8_t set_display_clock_div = 0xD5;
const uint8_t set_precharge = 0xD9;

const uint8_t set_multiplex = 0xA8;

const uint8_t set_low_column = 0x00;
const uint8_t set_high_column = 0x10;

const uint8_t set_start_line = 0x40;

const uint8_t memory_mode = 0x20;
const uint8_t column_addr = 0x21;
const uint8_t page_addr = 0x22;

const uint8_t com_scan_inc = 0xC0;
const uint8_t com_scan_dec = 0xC8;

const uint8_t seg_remap = 0xA0;

const uint8_t charge_pump = 0x8D;

const uint8_t external_vcc = 0x1;
const uint8_t switch_cap_vcc = 0x2;

const uint8_t activate_scroll = 0x2F;
const uint8_t deactivate_scroll = 0x2E;
const uint8_t set_vertical_scroll_area = 0xA3;
const uint8_t right_horizontal_scroll = 0x26;
const uint8_t left_horizontal_scroll = 0x27;
const uint8_t vertical_and_right_horizontal_scroll = 0x29;
const uint8_t vertical_and_left_horizontal_scroll = 0x2A;

const uint8_t command = 0x80; //Co=1, D/C#=0
const uint8_t data = 0x40; //Co=0, D/C#=1

uint8_t data_buf[WIDTH * HEIGHT / 8] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x40, 0x86, 0x80, 0x80, 0x86,
	0x40, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

ssd1306_setup init_ssd1306(uint8_t addr, bool on_off, bool inverted, uint8_t display_offset) {
	ssd1306_setup setup = {
		.addr = addr,
		.on_off = on_off,
		.inverted = inverted,
		.display_offset = display_offset,
	};

	uint8_t buf0[] = {command, display_off};
	uint8_t buf1[] = {command, set_display_clock_div, command, 0x80};
	uint8_t buf2[] = {command, set_multiplex, command, (uint8_t)(HEIGHT - 1)};
	uint8_t buf3[] = {command, set_display_offset, command, 0x00};
	uint8_t buf4[] = {command, set_start_line};
	uint8_t buf5[] = {command, charge_pump, command, 0x14};
	uint8_t buf6[] = {command, memory_mode, command, 0x00};
	uint8_t buf7[] = {command, seg_remap | 0x01};
	uint8_t buf8[] = {command, com_scan_dec};
	uint8_t buf9[] = {command, set_com_pins, command, 0x02};
	uint8_t buf10[] = {command, set_contrast, command, 0xFF};
	uint8_t buf11[] = {command, set_precharge, command, 0xF1};
	uint8_t buf12[] = {command, set_vcom_detect, command, 0x40};
	uint8_t buf13[] = {command, all_on};
	uint8_t buf14[] = {command, normal_display};
	uint8_t buf15[] = {command, deactivate_scroll};
	uint8_t buf16[] = {command, display_on};
	uint8_t buf17[] = {command, display_resume};


	i2c_write_blocking(i2c0, addr, buf0, 2, false);
	i2c_write_blocking(i2c0, addr, buf1, 4, false);
	i2c_write_blocking(i2c0, addr, buf2, 4, false);
	i2c_write_blocking(i2c0, addr, buf3, 4, false);
	i2c_write_blocking(i2c0, addr, buf4, 2, false);
	i2c_write_blocking(i2c0, addr, buf5, 4, false);
	i2c_write_blocking(i2c0, addr, buf6, 4, false);
	i2c_write_blocking(i2c0, addr, buf7, 2, false);
	i2c_write_blocking(i2c0, addr, buf8, 2, false);
	i2c_write_blocking(i2c0, addr, buf9, 4, false);
	i2c_write_blocking(i2c0, addr, buf10, 4, false);
	i2c_write_blocking(i2c0, addr, buf11, 4, false);
	i2c_write_blocking(i2c0, addr, buf12, 4, false);
	i2c_write_blocking(i2c0, addr, buf13, 2, false);
	i2c_write_blocking(i2c0, addr, buf14, 2, false);
	i2c_write_blocking(i2c0, addr, buf15, 2, false);
	i2c_write_blocking(i2c0, addr, buf16, 2, false);
	i2c_write_blocking(i2c0, addr, buf17, 2, false);
	uint8_t com[] = {command, column_addr, command, 0x00, command, (uint8_t)(127), command, page_addr, command, 0x00, command, 0x03};

	i2c_write_blocking(i2c0, setup.addr, com, 10, false);
	return setup;
}

void flush_data(ssd1306_setup *setup) {
	uint8_t buf[] = {data, 0x00};
	for (int i=0; i<(WIDTH * HEIGHT / 8); i++) {
		buf[1] = data_buf[i];
		i2c_write_blocking(i2c0, setup->addr, buf, 2, false);
	}
}

void write_char(char c, int x, int y) {
	if ((0 <= x) && (x < 127) && (0 <= y) && (y < 4)) {
		int character;
		switch ((uint8_t)(c)) {
			case (uint8_t)('a'): character = 0; break;
			case (uint8_t)('A'): character = 1; break;
			case (uint8_t)('b'): character = 2; break;
			case (uint8_t)('B'): character = 3; break;
			case (uint8_t)('c'): character = 4; break;
			case (uint8_t)('C'): character = 5; break;
			case (uint8_t)('d'): character = 6; break;
			case (uint8_t)('D'): character = 7; break;
			case (uint8_t)('e'): character = 8; break;
			case (uint8_t)('E'): character = 9; break;
			case (uint8_t)('f'): character = 10; break;
			case (uint8_t)('F'): character = 11; break;
			case (uint8_t)('g'): character = 12; break;
			case (uint8_t)('G'): character = 13; break;
			case (uint8_t)('h'): character = 14; break;
			case (uint8_t)('H'): character = 15; break;
			case (uint8_t)('i'): character = 16; break;
			case (uint8_t)('I'): character = 17; break;
			case (uint8_t)('j'): character = 18; break;
			case (uint8_t)('J'): character = 19; break;
			case (uint8_t)('k'): character = 20; break;
			case (uint8_t)('K'): character = 21; break;
			case (uint8_t)('l'): character = 22; break;
			case (uint8_t)('L'): character = 23; break;
			case (uint8_t)('m'): character = 24; break;
			case (uint8_t)('M'): character = 25; break;
			case (uint8_t)('n'): character = 26; break;
			case (uint8_t)('N'): character = 27; break;
			case (uint8_t)('o'): character = 28; break;
			case (uint8_t)('O'): character = 29; break;
			case (uint8_t)('p'): character = 30; break;
			case (uint8_t)('P'): character = 31; break;
			case (uint8_t)('q'): character = 32; break;
			case (uint8_t)('Q'): character = 33; break;
			case (uint8_t)('r'): character = 34; break;
			case (uint8_t)('R'): character = 35; break;
			case (uint8_t)('s'): character = 36; break;
			case (uint8_t)('S'): character = 37; break;
			case (uint8_t)('t'): character = 38; break;
			case (uint8_t)('T'): character = 39; break;
			case (uint8_t)('u'): character = 40; break;
			case (uint8_t)('U'): character = 41; break;
			case (uint8_t)('v'): character = 42; break;
			case (uint8_t)('V'): character = 43; break;
			case (uint8_t)('w'): character = 44; break;
			case (uint8_t)('W'): character = 45; break;
			case (uint8_t)('x'): character = 46; break;
			case (uint8_t)('X'): character = 47; break;
			case (uint8_t)('y'): character = 48; break;
			case (uint8_t)('Y'): character = 49; break;
			case (uint8_t)('z'): character = 50; break;
			case (uint8_t)('Z'): character = 51; break;
			case (uint8_t)(' '): character = 53; break;
			case (uint8_t)('-'): character = 54; break;
			case (uint8_t)('+'): character = 55; break;
			case (uint8_t)('\\'): character = 56; break;
			case (uint8_t)('/'): character = 57; break;
			case (uint8_t)('*'): character = 58; break;
			case (uint8_t)('.'): character = 59; break;
			case (uint8_t)('!'): character = 60; break;
			case (uint8_t)('1'): character = 61; break;
			case (uint8_t)('2'): character = 62; break;
			case (uint8_t)('3'): character = 63; break;
			case (uint8_t)('4'): character = 64; break;
			case (uint8_t)('5'): character = 65; break;
			case (uint8_t)('6'): character = 66; break;
			case (uint8_t)('7'): character = 67; break;
			case (uint8_t)('8'): character = 68; break;
			case (uint8_t)('9'): character = 69; break;
			case (uint8_t)('0'): character = 70; break;
			case (uint8_t)(':'): character = 71; break;
			case (uint8_t)('�'): character = 72; break;
			default: character = 52;
		}
		
		for (int i=0;i<6;i++) {
			if ((x + i) < 128) {
				data_buf[(y*128 + x + i)] = font[character][i];
			}
		}
	}
}	

void write_text(char *text, int x, int y) {
	for (int i=0;i<strlen(text);i++) {
		write_char(*(text+i), x+i*6, y);
	}
}