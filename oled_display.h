#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdbool.h>

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#ifndef I2C_PORT
#define I2C_PORT i2c0
#endif

#ifndef I2C_SDA
#define I2C_SDA 8
#endif

#ifndef I2CSCL
#define I2C_SCL 9
#endif

#define WIDTH 128
#define HEIGHT 32

#ifndef OLED
#define OLED
typedef struct {
	const uint8_t addr;
	bool on_off; //true=on, false=off
	bool inverted; //true=inverted, false=normal
	uint8_t display_offset;
	uint8_t display_clock_div;
	


} ssd1306_setup;

ssd1306_setup init_ssd1306(uint8_t addr, bool on_off, bool inverted, uint8_t display_offset);
void fill_screen(ssd1306_setup *);
void continue_gddram(ssd1306_setup *);
void flush_data(ssd1306_setup *);
void write_text(char *text, int x, int y);
void write_char(char c, int x, int y);
void invert_display(ssd1306_setup *);
void write_int(int n, int x, int y);


#endif
