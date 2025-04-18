################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/abcc-driver-api/src/abcc_api_command_handler.c \
../lib/abcc-driver-api/src/abcc_api_handler.c \
../lib/abcc-driver-api/src/abcc_api_select_firmware.c 

OBJS += \
./lib/abcc-driver-api/src/abcc_api_command_handler.o \
./lib/abcc-driver-api/src/abcc_api_handler.o \
./lib/abcc-driver-api/src/abcc_api_select_firmware.o 

C_DEPS += \
./lib/abcc-driver-api/src/abcc_api_command_handler.d \
./lib/abcc-driver-api/src/abcc_api_handler.d \
./lib/abcc-driver-api/src/abcc_api_select_firmware.d 


# Each subdirectory must supply rules for building sources it contributes
lib/abcc-driver-api/src/%.o lib/abcc-driver-api/src/%.su lib/abcc-driver-api/src/%.cyclo: ../lib/abcc-driver-api/src/%.c lib/abcc-driver-api/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H7xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc/anybus_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc/host_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src/host_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src/anybus_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/par" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/serial" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/spi" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/abcc-abp/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/src/abcc_adaptation" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/src/example_application" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib-2f-abcc-2d-driver-2d-api-2f-src

clean-lib-2f-abcc-2d-driver-2d-api-2f-src:
	-$(RM) ./lib/abcc-driver-api/src/abcc_api_command_handler.cyclo ./lib/abcc-driver-api/src/abcc_api_command_handler.d ./lib/abcc-driver-api/src/abcc_api_command_handler.o ./lib/abcc-driver-api/src/abcc_api_command_handler.su ./lib/abcc-driver-api/src/abcc_api_handler.cyclo ./lib/abcc-driver-api/src/abcc_api_handler.d ./lib/abcc-driver-api/src/abcc_api_handler.o ./lib/abcc-driver-api/src/abcc_api_handler.su ./lib/abcc-driver-api/src/abcc_api_select_firmware.cyclo ./lib/abcc-driver-api/src/abcc_api_select_firmware.d ./lib/abcc-driver-api/src/abcc_api_select_firmware.o ./lib/abcc-driver-api/src/abcc_api_select_firmware.su

.PHONY: clean-lib-2f-abcc-2d-driver-2d-api-2f-src

