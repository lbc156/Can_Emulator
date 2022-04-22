################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/CMSIS/CM3/CoreSupport/core_cm3.c 

OBJS += \
./Libraries/CMSIS/CM3/CoreSupport/core_cm3.o 

C_DEPS += \
./Libraries/CMSIS/CM3/CoreSupport/core_cm3.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/CMSIS/CM3/CoreSupport/%.o: ../Libraries/CMSIS/CM3/CoreSupport/%.c Libraries/CMSIS/CM3/CoreSupport/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\App" -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\Libraries\CMSIS\CM3\CoreSupport" -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -isystem"D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\Libraries\STM32F10x_StdPeriph_Driver\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


