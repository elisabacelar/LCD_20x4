# LCD_20x4

This is a HD44780 20x4 LCD API for stm32 using STM32F4 HAL that supports 4 bit mode operation.

**Attention:** To use this library in System Workbench for STM32 you must enable the flag `-u _printf_float`. This link explains how to do it: https://www.openstm32.org/forumthread3351

## Introduction
This API was developed as a work in the discipline of Embedded Systems Programming at UFMG - Prof. Ricardo de Oliveira Duarte - Department of Electronic Engineering
The Library is developed and tested with Stm32F401RE (Nucleo 64 board). You can check the examples given.

## Example
In main.c:
```c
#include "lcd.h"
```
```c
GPIO_Port ports[] = {D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port};
GPIO_Pin pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};

DisplayLCD lcd = lcd_generate(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin);

lcd_init(&lcd);

lcd_write_data(&lcd,"Hello World!");
```

## API functions:

### void lcd_clear_display(DisplayLCD* lcd)
Clears everything that is currently written on the Display and returns the cursor to (0,0).

### void lcd_display_matrix (DisplayLCD* lcd)
This function can be used to reprint the whole display using the mirror matrix as reference. As parameter, this function requires the LCD object, because the mirror matrix is stored inside it.

### void lcd_fast_shift(DisplayLCD* lcd, uint8_t direction, uint8_t times, uint16_t delay)
Performs the shift instruction from the HD44780.The direction of the shift, the number of shifted positions and the delay applied between those shifts must be provided.

This shift is performed much faster than lcd_shift_display, since it is an operation from the controller. 

For this function, there are two sets of rows treated separately: ROW 0 together with ROW 2, and ROW 1 together with ROW 3. Within each set, the rows are arrangeded in a continuous and cyclic manner. That is, after reaching the end of a row, the cursor will go to the beggining of the other row of the set.

In this function, a shift is performed in each of the sets at the same time. The amount of shifts that are performed is defined by *times* and, between each shift operation, there is a delay defined by *delay*. In the end of the process, the display returns to its original state and the cursor goes to (0,0). 

**Example:**
|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|E|1|
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|E|2|
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
```
lcd_fast_shift&lcd,RIGHT,1,1000);
```
|_|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|E|
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|_|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|E|
|1|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
|2|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|

### DisplayLCD lcd_generate (GPIO_Port data_port[], GPIO_Pin data_pin[],GPIO_Port rs_port, GPIO_Pin rs_pin, GPIO_Port en_port,GPIO_Pin en_pin)
This function instantiate a DisplayLCD object. In order to create the object, it is necessary to provide:
* an array of GPIO_Port with the ports of the data pins;
* an array of GPIO_Pin with the data pins;
* GPIO_Port of the register pin (RS);
* GPIO_Pin of the register pin (RS);
* GPIO_Port of the enable pin (EN);
* GPIO_Pin of the enable pin (EN).

### void lcd_init(DisplayLCD* lcd)
It is necessary to use this function to do the initial setup of display. The function first sends some signals to start the four-bit mode, then starts the display and the cursor. The only parameter required to use this function is the LCD object that will be configured.

### void lcd_pos_cursor(DisplayLCD* lcd,Coordinates coordinates)
Moves the cursor cursor to a position passed as a parameter to the function. This position must be passed as a coordinate. In addition, the display on which the cursor is to be moved must be passed as an LCD object. This function differs from the lcd_shift_cursor because the latter only moves the cursor one position to the left or right while this function can move the cursor to any position on the display. Some writing functions start to write from the current cursor position (e.g. lcd_write_data). Therefore, using this to place the cursor in a position of interest can be useful before calling these other functions.

### char lcd_read_from_matrix (DisplayLCD* lcd,Coordinates coordinates,PosConfig mode)
Returns a character read from the mirror matrix. With this function, it is possible to either use the current position of the cursor or specify a position of the display to retrieve the character stored in it. To use it, it is necessary to pass the LCD object that will be read, the position to be read and a flag to indicate whether the passed position should be used (SET_MODE) or not (CURSOR_MODE). If not, the function will use the cursor position.

### void lcd_return_home (DisplayLCD* lcd)
Useful function to establish a known state of the LCD. The function sets the cursor position and the origin of the display to the upper left corner of the screen. It does not change the cursor data, but can move it if the LCD origin is not in the upper left corner. The function requires the LCD object to which it will apply.

### void lcd_set_to_matrix (DisplayLCD* lcd,Coordinates coordinates,PosConfig mode,char character)
It is quite unusual to use the mirror matrix to store data without displaying it on the LCD. However, if an application requires that, this function can be used to add data only to the matrix. The position in which the data will be inserted can be specified or not. In the negative situation, the function will use the cursor position. The parameters required by the function are the LCD object, whose matrix will be used, the coordinates where the data will be stored, the character that will be written to the matrix and a flag to indicate whether the cursor position (CURSOR_MODE) or the position set (SET_MODE) will be used. Note that some functions use the mirror matrix to print data on the LCD (e.g. lcd_display_matrix, lcd_shift_display). Therefore, if the information stored in the matrix cannot be displayed on the LCD, these functions should not be used in conjunction with this one. 

### void lcd_shift_cursor(DisplayLCD* lcd,Direction direction)
Shifts the cursor in one position for the right or for the left. The direction of the switch must be provided.

### void lcd_shift_display(DisplayLCD* lcd,Direction direction)
Shifts what is written on the Display by one position to the left or to the right. The direction of the shift must be provided. 

This function is much slower than lcd_fast_shift. However, for this shift, all four rows are continuous and cyclic. That is, when the string reaches the end of a row, it continues on the following row, and if it happens in the last row, it goes all the way back to the first row.

**Example:**
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|_|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|E|
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|

```
lcd_shift_display(&lcd,RIGHT);
```
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|
|E|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|


### void lcd_write_char(DisplayLCD* lcd,Coordinates coordinates,char character)
Writes a character to the LCD and to the mirror matrix. Passing a char to the string in the lcd_write_data results is unexpected behaviour. This function exists to give support to that sittuation then. Also, it is usefull to write special characters to the LCD if it is used together with the defined special characters names in lcd.h. The parameters of this function include the LCD object that will be written to, the position (coordinates) where the character should be written and the character itself.

### void lcd_write_data(DisplayLCD* lcd,char* string)
Writes *string* on the display in the current position of the cursor. 

The rows of the display are continuous. It means that, in case the string reaches the end of a row, it continues in the following row. When it is in the last row, it continues in the first row. Therefore, the rows are not only continuous but cyclic.

### void lcd_write_float(DisplayLCD* lcd, float number)
Writes a float number with three decimals on the display in the current position of the cursor.


## Non-public functions:

### void lcd_load(DisplayLCD* lcd,uint8_t data,uint8_t time,LoadMode mode)
This function is not in lcd.h because it is not necessary to understand it to use the API. Understanding this function is useful for improving it or making changes to the API. It basically divides a given instruction / data into 2 nibbles and sends them together with the necessary signals to load that instruction / data to the LCD. Also, it applies the necessary delay for each command to take effect. The instructions / data need to be divided into 2 nibbles because, in this API, the LCD is working in 4-bit mode. For it to work, it is necessary to pass the LCD object that will be used, the instruction / data that will be loaded on it, the amount of delay required for that command and a flag to indicate whether the signal in the data parameter is instruction or data.

