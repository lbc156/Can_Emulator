################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Libraries/STM32F10x_StdPeriph_Driver/src/subdir.mk
-include Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/subdir.mk
-include Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/subdir.mk
-include Libraries/CMSIS/CM3/CoreSupport/subdir.mk
-include App/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Can_Vector_Emulation
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Can_Vector_Emulation.hex \

SECONDARY_SIZE += \
Can_Vector_Emulation.siz \


# All Target
all: main-build

# Main-build Target
main-build: Can_Vector_Emulation.elf secondary-outputs

# Tool invocations
Can_Vector_Emulation.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T "D:\Workspace\Can_Vector_Emulation_Workspace_Eclipse\Can_Vector_Emulation\Linker\gcc.ld" -Xlinker --gc-sections -Wl,-Map,"Can_Vector_Emulation.map" --specs=nosys.specs -o "Can_Vector_Emulation.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Can_Vector_Emulation.hex: Can_Vector_Emulation.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU Arm Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "Can_Vector_Emulation.elf"  "Can_Vector_Emulation.hex"
	@echo 'Finished building: $@'
	@echo ' '

Can_Vector_Emulation.siz: Can_Vector_Emulation.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU Arm Cross Print Size'
	arm-none-eabi-size --format=berkeley "Can_Vector_Emulation.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) Can_Vector_Emulation.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
