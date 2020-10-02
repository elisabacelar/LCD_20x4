/*
 * ***************************************************************
 * file		    : lcd.c
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

#include "lcd.h"

/************************************** Static declarations **************************************/

const uint8_t lcd_row_addresses[] = {0x00,0x40,0x14,0x54};
const uint8_t lcd_row_positions[] = {0,2,1,3};
static void lcd_load(DisplayLCD* lcd, uint8_t data, uint8_t time, LoadMode mode);

///************************************** Function definitions **************************************/
// header das funções
DisplayLCD lcd_generate(GPIO_Port port[],GPIO_Pin pin[],
GPIO_Port rs_port,GPIO_Pin rs_pin,GPIO_Port en_port,
GPIO_Pin en_pin) {
	DisplayLCD lcd;

	lcd.en_pin = en_pin;
	lcd.en_port = en_port;

	lcd.rs_pin = rs_pin;
	lcd.rs_port = rs_port;

	lcd.data_pin = pin;
	lcd.data_port = port;

	return lcd;
}

void lcd_init(DisplayLCD* lcd) {

	lcd->cursor_pos[0] = 0;
	lcd->cursor_pos[1] = 0;

	for(uint8_t it=0;it<4;++it) {
		for(uint8_t it2=0;it2<20;++it2) {
			lcd->display.matrix[it][it2] = ' ';
		}
	}

    HAL_Delay(15);
	lcd_load(lcd,0x33,LARGE_DELAY,LOAD_INSTRUCTION);
	lcd_load(lcd,0x32,LARGE_DELAY,LOAD_INSTRUCTION);

	lcd_load(lcd,FUNCTION_SET | OPT_N,SHORT_DELAY,LOAD_INSTRUCTION);

	lcd_load(lcd,DISPLAY_ON_OFF_CONTROL,SHORT_DELAY,LOAD_INSTRUCTION);
	lcd_load(lcd,CLEAR_DISPLAY,MEDIUM_DELAY,LOAD_INSTRUCTION);

	lcd_load(lcd,DISPLAY_ON_OFF_CONTROL | OPT_D | OPT_C | OPT_B,SHORT_DELAY,LOAD_INSTRUCTION);
}

void lcd_clear_display(DisplayLCD* lcd) {
	lcd_load(lcd,CLEAR_DISPLAY,MEDIUM_DELAY,LOAD_INSTRUCTION);

	lcd->cursor_pos[0] = 0;
	lcd->cursor_pos[1] = 0;

	for(uint8_t it=0;it<4;++it) {
		for(uint8_t it2=0;it2<20;++it2) {
			lcd->display.matrix[it][it2] = ' ';
		}
	}

}

void lcd_return_home(DisplayLCD* lcd) {
	lcd_load(lcd,RETURN_HOME,MEDIUM_DELAY,LOAD_INSTRUCTION);

	lcd->cursor_pos[0] = 0;
	lcd->cursor_pos[1] = 0;
}

void lcd_shift_cursor(DisplayLCD* lcd,Direction direction) {
	Coordinates pos = {lcd->cursor_pos[0],lcd->cursor_pos[1]};

	if(direction == LEFT) {

		if(lcd->cursor_pos[0]==0) {

			pos[0] = 19;

			switch(lcd->cursor_pos[1]) {
				case 0:
					pos[1] = 3;
					break;
				case 1:
					pos[1] = 0;
					break;
				case 2:
					pos[1] = 1;
					break;
				case 3:
					pos[1] = 2;
					break;
				default:
					pos[1] = 0;
			}

		} else {

		    --pos[0];
		}
	} else {
	    if(lcd->cursor_pos[0] == 19) {
		    pos[0] = 0;
		    switch(pos[1]) {
			    case 0:
			        pos[1] = 1;
			        break;
			    case 1:
				    pos[1] = 2;
				    break;
			    case 2:
				    pos[1] = 3;
			        break;
			    case 3:
				    pos[1] = 0;
				    break;
			    default:
				    pos[1] = 0;
		    }
	    } else {
	    	++pos[0];
	    }
	}
	lcd_pos_cursor(lcd,pos);
}

void lcd_fast_shift(DisplayLCD* lcd,uint8_t direction,uint8_t
		            times, uint16_t delay) {
	for(uint8_t it=0;it<times;++it) {
	    lcd_load(lcd,DISPLAY_SHIFT|(direction << 2),SHORT_DELAY,LOAD_INSTRUCTION);
	    HAL_Delay(delay);
	}
	lcd_return_home(lcd);
}

void lcd_shift_display(DisplayLCD* lcd,Direction direction) {
	DisplayMatrix original_display = lcd->display;
	Coordinates original_pos = {lcd->cursor_pos[0],lcd->cursor_pos[1]};
	lcd_clear_display(lcd);
	lcd_shift_cursor(lcd, direction);
	for(uint8_t i = 0; i < 4; i++) {
		for (uint8_t j = 0; j < 20; j++) {
			char str[1];
			str[0] = original_display.matrix[i][j];
			lcd_write_data(lcd,str);
		}
	}
	lcd_pos_cursor(lcd,original_pos);
	lcd_shift_cursor(lcd, direction);

}

void lcd_shift_display_matrix(DisplayLCD* lcd, Direction direction) {
	DisplayMatrix original_display = lcd->display;
	Coordinates original_pos = {lcd->cursor_pos[0],lcd->cursor_pos[1]};
	lcd_clear_display(lcd);
	char str[1];

	switch(direction) {
	case LEFT:
		for(uint8_t i=0; i<4; ++i) {
			lcd_pos_cursor(lcd,(Coordinates) {19,i});
			str[0] = original_display.matrix[i][0];
			lcd_write_data(lcd,str);
			for(uint8_t j=0;j<19;++j) {
				lcd_pos_cursor(lcd,(Coordinates) {j,i});
				str[0] = original_display.matrix[i][j+1];
				lcd_write_data(lcd,str);
			}
		}
		break;
	default:
		for(uint8_t i=0; i<4; ++i) {
			lcd_pos_cursor(lcd,(Coordinates) {0,i});
			str[0] = original_display.matrix[i][19];
			lcd_write_data(lcd,str);
			for(uint8_t j=1;j<20;++j) {
				lcd_pos_cursor(lcd,(Coordinates) {j,i});
				str[0] = original_display.matrix[i][j-1];
				lcd_write_data(lcd,str);
			}
		}
	}
	lcd_pos_cursor(lcd,original_pos);
	lcd_shift_cursor(lcd, direction);
}

void lcd_pos_cursor(DisplayLCD* lcd,Coordinates coordinates) {
    lcd_load(lcd,SET_DDRAM_ADDR|(lcd_row_addresses[coordinates[1]]+
    coordinates[0]),SHORT_DELAY,LOAD_INSTRUCTION);
    lcd->cursor_pos[0] = coordinates[0];
    lcd->cursor_pos[1] = coordinates[1];
}

void lcd_display_matrix(DisplayLCD* lcd) {
	Coordinates old_pos = {lcd->cursor_pos[0],
	lcd->cursor_pos[1]};
	lcd_load(lcd,RETURN_HOME,MEDIUM_DELAY,LOAD_INSTRUCTION);
	for(uint8_t it=0;it<4;++it) {
		for(uint8_t it2=0;it2<20;++it2) {
			lcd_load(lcd,lcd->display.matrix[lcd_row_positions[it]]
			[it2],SHORT_DELAY,LOAD_DATA);
	    }
	}
	lcd_pos_cursor(lcd,old_pos);
}

void lcd_write_data(DisplayLCD* lcd,char* string) {
	for(uint8_t i = 0; i < strlen(string); i++) {
		lcd_load(lcd, string[i], SHORT_DELAY, LOAD_DATA);
		lcd->display.matrix[lcd->cursor_pos[1]][lcd->cursor_pos[0]] = string[i];
		lcd_shift_cursor(lcd, RIGHT);
	}
}

///************************************** Static function definition **************************************/

void lcd_load(DisplayLCD* lcd,uint8_t data,uint8_t time,
		      LoadMode mode)
{
	HAL_GPIO_WritePin(lcd->rs_port,lcd->rs_pin,mode);
	for(uint8_t it=0;it<4;++it) {
		HAL_GPIO_WritePin(lcd->data_port[it],
		lcd->data_pin[it],(data>>(it+4))&0x01);
	}
	HAL_GPIO_WritePin(lcd->en_port,lcd->en_pin,1);
	HAL_Delay(time);
	HAL_GPIO_WritePin(lcd->en_port,lcd->en_pin,0);
	HAL_Delay(time);
	for(uint8_t it=0;it<4;++it) {
		HAL_GPIO_WritePin(lcd->data_port[it],
		lcd->data_pin[it],(data>>it)&0x01);
	}
    HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin,1);
	HAL_Delay(time);
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin,0);
	HAL_Delay(time);
}
