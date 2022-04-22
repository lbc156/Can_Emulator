################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/main.c \
../App/stm32f10x_it.c 

OBJS += \
./App/main.o \
./App/stm32f10x_it.o 

C_DEPS += \
./App/main.d \
./App/stm32f10x_it.d 


# Each subdirectory must supply rules for building sources it contributes
App/%.o: ../App/%.c App/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\App" -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\Libraries\CMSIS\CM3\CoreSupport" -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\Libraries\STM32F10x_StdPeriph_Driver\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


