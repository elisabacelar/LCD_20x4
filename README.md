# LCD_20x4
This a HD44780 20x4 LCD API for stm32 using STM32F4 HAL. It was created as an assignement for Embbeded Systems Programming class at the Federal University of Minas Gerais.

**Attention:** To use this library in System Workbench for STM32 you must enable the flag `-u _printf_float`. This link explains how to do it: https://www.openstm32.org/forumthread3351

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

### void lcd_write_data(DisplayLCD* lcd,char* string)
Writes *string* on the display in the current position of the cursor. 

The rows of the display are continuous. It means that, in case the string reaches the end of a row, it continues in the following row. When it is in the last row, it continues in the first row. Therefore, the rows are not only continuous but cyclic. 

### void lcd_shift_cursor(DisplayLCD* lcd,Direction direction)
Shifts the cursor in one position for the right or for the left. The direction of the switch must be provided. 

This shift is compliant to the continuous/cyclic arrangement of the rows.

### void lcd_write_float(DisplayLCD* lcd, float number)
Writes a float number with three decimals on the display in the current position of the cursor.

### void lcd_shift_display(DisplayLCD* lcd,Direction direction)
Shifts everything that is written on the Display by one position to the left or to the right. The direction of the shift must be provided. 

This shift is compliant to the continuous/cyclic arrangement of the rows.

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

### void lcd_parallel_shift(DisplayLCD* lcd, Direction direction)
Shifts the rows of the Display in parallel by one position to the left or to the right. The direction of the shift must be provided.

This shift is not compliant to the continuos/cyclic arrangement of the rows. In this case, the rows are not continuous and a row is cyclic with respect to itself. 
That is, a character that is the end of a row, when shifted to the right, will appear in the beggining of the same row.

**Example:**
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|E|1|
|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|E|2|
|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|E|3|

```
lcd_shift_parallel_shift(&lcd,RIGHT);
```
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|1|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|E|
|2|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|E|
|3|_|_|_|_|_|_|_|_|_|_|_|_|E|X|A|M|P|L|E|

### void lcd_fast_shift(DisplayLCD* lcd, uint8_t direction, uint8_t times, uint16_t delay)
Performs the shift instruction from the HD44780.The direction of the shift, the number of shifted positions and the delay applied bwtween those shifts must be provided.

This shift is performed much faster than the other display shift functions (lcd_parallel_shift and lcd_shift_display). It is not compliant to the continuos/cyclic arrangement of the rows. In this function, there are two sets of rows treated separately: ROW 0 together with ROW 2, and ROW 1 together with ROW 3. Within each set, the rows are arrangeded in a continuous and cyclic manner. 

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
