################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/example_application/subdir.mk
-include src/abcc_adaptation/subdir.mk
-include lib/abcc-driver-api/src/host_objects/subdir.mk
-include lib/abcc-driver-api/src/anybus_objects/subdir.mk
-include lib/abcc-driver-api/src/subdir.mk
-include lib/abcc-driver-api/abcc-driver/src/spi/subdir.mk
-include lib/abcc-driver-api/abcc-driver/src/serial/subdir.mk
-include lib/abcc-driver-api/abcc-driver/src/par/subdir.mk
-include lib/abcc-driver-api/abcc-driver/src/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/STM32H7xx_Nucleo/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
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


BUILD_ARTIFACT_NAME := nucleo-h753zi2_m00765_cubeide
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
nucleo-h753zi2_m00765_cubeide.elf \

MAP_FILES += \
nucleo-h753zi2_m00765_cubeide.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
nucleo-h753zi2_m00765_cubeide.list \


# All Target
all: main-build

# Main-build Target
main-build: nucleo-h753zi2_m00765_cubeide.elf secondary-outputs

# Tool invocations
nucleo-h753zi2_m00765_cubeide.elf nucleo-h753zi2_m00765_cubeide.map: $(OBJS) $(USER_OBJS) C:\Users\ajoh\unixhome\github\abcc-example-stm32nucleo\STM32H753ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "nucleo-h753zi2_m00765_cubeide.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\ajoh\unixhome\github\abcc-example-stm32nucleo\STM32H753ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="nucleo-h753zi2_m00765_cubeide.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

nucleo-h753zi2_m00765_cubeide.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "nucleo-h753zi2_m00765_cubeide.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout nucleo-h753zi2_m00765_cubeide.elf nucleo-h753zi2_m00765_cubeide.list nucleo-h753zi2_m00765_cubeide.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
