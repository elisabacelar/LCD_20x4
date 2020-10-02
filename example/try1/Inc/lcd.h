/*
 * ***************************************************************
 * file		: lcd.h
 *  authors		: Elisa Bacelar, Gabriel Araújo
 *  date        : 09/29/20
 *  modified	: 09/29/20
 *  This code implements a library for LCD 20x4
 ***************************************************************
 *  HARDWARE SETUP:
 *  VSS = GND
 *  VDD = 5v
 *  VO is connected to a potentiometer to adjust the backlight
 *  RS = D8
 *  RW = GND
 *  E = D9
 *  D0 = -
 *  D1 = -
 *  D2 = -
 *  D3 = -
 *  D4 = D4
 *  D5 = D5
 *  D6 = D6
 *  D7 = D7
 *  A = 5v
 *  K = GND
 *****************************************************************
 */

#ifndef LCD_H
#define LCD_H

#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include "main.h"


///************************************** Command register **************************************/
#define CLEAR_DISPLAY 0x01

#define RETURN_HOME 0x02
//
//#define ENTRY_MODE_SET 0x04
//#define OPT_S	0x01					// Shift entire display to right
//#define OPT_INC 0x02					// Cursor increment
//
#define DISPLAY_ON_OFF_CONTROL 0x08
#define OPT_D	0x04					// Turn on display
#define OPT_C	0x02					// Turn on cursor
#define OPT_B 	0x01					// Turn on cursor blink
//
#define CURSOR_SHIFT 0x10		// Move and shift cursor
#define DISPLAY_SHIFT 0x18
//#define OPT_SC 0x08
//#define OPT_RL 0x04
//
#define FUNCTION_SET 0x20
#define OPT_DL 0x10						// Set interface data length
#define OPT_N 0x08						// Set number of display lines
#define OPT_F 0x04						// Set alternate font
//#define SETCGRAM_ADDR 0x040
#define SET_DDRAM_ADDR 0x80				// Set DDRAM address
//
//
///************************************** Helper macros **************************************/
#define SHORT_DELAY 1
#define MEDIUM_DELAY 2
#define LARGE_DELAY 5


///************************************** LCD defines **************************************/
//#define LCD_NIB 4
//#define LCD_BYTE 8
//#define LCD_DATA_REG 1
//#define LCD_COMMAND_REG 0
//
//
///************************************** LCD typedefs **************************************/
typedef uint8_t Coordinates[2];
typedef GPIO_TypeDef* GPIO_Port;
typedef uint16_t GPIO_Pin;

typedef enum {
	LEFT,
	RIGHT
} Direction;

typedef enum {
	LOAD_INSTRUCTION,
	LOAD_DATA
} LoadMode;

typedef struct {
	char matrix[4][20];
} DisplayMatrix;

typedef struct {
	GPIO_Port* data_port;
	GPIO_Pin* data_pin;

	GPIO_Port rs_port;
	GPIO_Pin rs_pin;

	GPIO_Port en_port;
	GPIO_Pin en_pin;

    Coordinates cursor_pos;
    DisplayMatrix display;

} DisplayLCD;


/************************************** Public functions **************************************/
void lcd_init(DisplayLCD* lcd);
void lcd_clear_display(DisplayLCD* lcd);
void lcd_display_matrix(DisplayLCD* lcd);
void lcd_fast_shift(DisplayLCD* lcd, uint8_t direction,
		uint8_t times, uint16_t delay);
DisplayLCD lcd_generate(GPIO_Port data_port[], GPIO_Pin data_pin[],
		GPIO_Port rs_port, GPIO_Pin rs_pin, GPIO_Port en_port,
		GPIO_Pin en_pin);
void lcd_pos_cursor(DisplayLCD* lcd,uint8_t coordinates[2]);
void lcd_return_home(DisplayLCD* lcd);
void lcd_shift_cursor(DisplayLCD* lcd,Direction direction);
void lcd_shift_display(DisplayLCD* lcd,Direction direction);
void lcd_shift_display_matrix(DisplayLCD* lcd, Direction direction);
void lcd_write_data(DisplayLCD* lcd,char* string);

#endif /* LCD_H */
