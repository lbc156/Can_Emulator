################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/STM32F10x_StdPeriph_Driver/src/misc.c \
../Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c \
../Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c \
../Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c 

OBJS += \
./Libraries/STM32F10x_StdPeriph_Driver/src/misc.o \
./Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.o \
./Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.o \
./Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.o 

C_DEPS += \
./Libraries/STM32F10x_StdPeriph_Driver/src/misc.d \
./Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.d \
./Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.d \
./Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/STM32F10x_StdPeriph_Driver/src/%.o: ../Libraries/STM32F10x_StdPeriph_Driver/src/%.c Libraries/STM32F10x_StdPeriph_Driver/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\App" -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\Libraries\CMSIS\CM3\CoreSupport" -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\Libraries\STM32F10x_StdPeriph_Driver\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


