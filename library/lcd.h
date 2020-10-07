/*
 * ***************************************************************
 *  API name    : LCD20X4
 *  API files   : lcd.c, lcd.h
 *  file		: lcd.h
 *  authors		: Elisa Bacelar, Gabriel Araújo
 *  university  : Federal University of Minas Gerais
 *  license     : GNU General Public License v3.0
 *  date        : 09/29/20
 *  modified	: 09/29/20
 *  This code implements a library for LCD 20x4
 *
 *  This API was developed as an assignment for Embedded Systems
 *  Programming class at the Federal University of Minas Gerais
 ***************************************************************
 *  SOFTWARE SETUP:
 *  Include lcd.h in main.c
 *
 *  Enable the flag -u _printf_float in System Workbench for STM32.
 *  More information in: https://www.openstm32.org/forumthread3351
 *
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
#define DISPLAY_ON_OFF_CONTROL 0x08
#define OPT_D	0x04					// Turn on display
#define OPT_C	0x02					// Turn on cursor
#define OPT_B 	0x01					// Turn on cursor blink
#define CURSOR_SHIFT 0x10				// Move and shift cursor
#define DISPLAY_SHIFT 0x18
#define FUNCTION_SET 0x20
#define OPT_DL 0x10						// Set interface data length
#define OPT_N 0x08						// Set number of display lines
#define OPT_F 0x04						// Set alternate font
#define SETCGRAM_ADDR 0x040
#define SET_DDRAM_ADDR 0x80				// Set DDRAM address
//
//
//Special characters definitions
#define LCD_ALPHA 0xE0                      //𝛼
#define LCD_BETA 0xE2                       //β
#define LCD_BLACK_RECTANGLE 0xFF            //◼️
#define LCD_DIVISION_SIGN 0xFD              //÷
#define LCD_EPSILON 0xE3                    //ε
#define LCD_INFINITY 0xF3                   //∞
#define LCD_LEFT_ARROW 0x7F                 //￩
#define LCD_MU 0xE4                         //μ
#define LCD_OMEGA 0xF4                      //Ω
#define LCD_PI 0xF7                         //π
#define LCD_RHO 0xE6                        //ρ
#define LCD_RIGHT_ARROW 0x7E                //￫
#define LCD_SIGMA 0xE5                      //𝜎
#define LCD_SUMMATION 0xF6                  //Σ
#define LCD_THETA 0xF2                      //θ
#define LCD_YEN 0x5C                        //¥

///************************************** Helper macros **************************************/
#define SHORT_DELAY 1
#define MEDIUM_DELAY 2
#define LONG_DELAY 5

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

typedef enum {
	CURSOR_MODE,
	SET_MODE
} PosConfig;

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
void lcd_clear_display(DisplayLCD* lcd);
void lcd_display_matrix(DisplayLCD* lcd);
void lcd_fast_shift(DisplayLCD* lcd, uint8_t direction,
		uint8_t times, uint16_t delay);
DisplayLCD lcd_generate(GPIO_Port data_port[], GPIO_Pin data_pin[],
		GPIO_Port rs_port, GPIO_Pin rs_pin, GPIO_Port en_port,
		GPIO_Pin en_pin);
void lcd_init(DisplayLCD* lcd);
void lcd_pos_cursor(DisplayLCD* lcd,Coordinates coordinates);
char lcd_read_from_matrix(DisplayLCD* lcd,Coordinates coordinates,PosConfig mode);
void lcd_return_home(DisplayLCD* lcd);
void lcd_set_to_matrix(DisplayLCD* lcd,Coordinates coordinates,PosConfig mode,char character);
void lcd_shift_cursor(DisplayLCD* lcd,Direction direction);
void lcd_shift_display(DisplayLCD* lcd,Direction direction);
void lcd_write_char(DisplayLCD* lcd,Coordinates coordinates,char character);
void lcd_write_data(DisplayLCD* lcd,char* string);
void lcd_write_float(DisplayLCD* lcd, float number);
#endif /* LCD_H */
