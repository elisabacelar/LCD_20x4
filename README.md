# LCD_20x4
This a HD44780 20x4 LCD API for stm32 using STM32F4 HAL.

## API functions:
### DisplayLCD lcd_generate (GPIO_Port data_port[], GPIO_Pin data_pin[],GPIO_Port rs_port, GPIO_Pin rs_pin, GPIO_Port en_port,GPIO_Pin en_pin)
This function instantiate a DisplayLCD object. In order to create the object, it is necessary to provide:
* an array of GPIO_Port with the ports from the data pins;
* an array of GPIO_Pin with the data pins;
* GPIO_Port of the register pin (RS);
* GPIO_Pin of the register pin (RS);
* GPIO_Port of the enable pin (EN);
* GPIO_Pin of the enable pin (EN).

### void lcd_clear_display(DisplayLCD* lcd)
Clears everything that is currently written on the Display and returns the cursor to (0,0).

### void lcd_shift_cursor(DisplayLCD* lcd,Direction direction)
Shifts the cursor in one position for the right or for the left. The direction of the switch must be provided. 

In case the cursor is in the end of a row and is shifted to the right, it will be positioned in the beggining of the following row. If it is the last row, it will go all the way back to the beggining of the first row. Likewise, in case the cursor is in the beggining of a row and it is shifted to the left, it will be positioned in the end of the previous row, and if the current row is the first row of the display, the cursor will go to the end of the last row.

### void lcd_fast_shift(DisplayLCD* lcd, uint8_t direction, uint8_t times, uint16_t delay)

### void lcd_write_data(DisplayLCD* lcd,char* string)
Writes *string* on the display in the current position of the cursor. 

### void lcd_write_float(DisplayLCD* lcd, float number)
Writes a float number on the display in the current position of the cursor. To use this function in Eclipse you should enable the flag Markup : `-u _printf_float`. This link explains how to do it: https://www.openstm32.org/forumthread3351

###
