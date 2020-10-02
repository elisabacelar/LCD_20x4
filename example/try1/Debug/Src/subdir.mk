################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/lcd.c \
../Src/main.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f4xx.c 

OBJS += \
./Src/lcd.o \
./Src/main.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f4xx.o 

C_DEPS += \
./Src/lcd.d \
./Src/main.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F401xE -I"/home/elisa/Documents/2020-1/ProgSisEmb/Trabalho1/try1/try1/Inc" -I"/home/elisa/Documents/2020-1/ProgSisEmb/Trabalho1/try1/try1/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/elisa/Documents/2020-1/ProgSisEmb/Trabalho1/try1/try1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/home/elisa/Documents/2020-1/ProgSisEmb/Trabalho1/try1/try1/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/elisa/Documents/2020-1/ProgSisEmb/Trabalho1/try1/try1/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


