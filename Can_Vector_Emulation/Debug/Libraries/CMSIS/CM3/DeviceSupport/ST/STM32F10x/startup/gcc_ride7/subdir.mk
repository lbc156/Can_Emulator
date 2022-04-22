################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_ARMCM3.S 

OBJS += \
./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_ARMCM3.o 

S_UPPER_DEPS += \
./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_ARMCM3.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/%.o: ../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/%.S Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


