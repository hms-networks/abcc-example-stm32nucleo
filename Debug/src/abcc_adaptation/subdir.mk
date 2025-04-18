################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/abcc_adaptation/abcc_hardware_abstraction.c 

OBJS += \
./src/abcc_adaptation/abcc_hardware_abstraction.o 

C_DEPS += \
./src/abcc_adaptation/abcc_hardware_abstraction.d 


# Each subdirectory must supply rules for building sources it contributes
src/abcc_adaptation/%.o src/abcc_adaptation/%.su src/abcc_adaptation/%.cyclo: ../src/abcc_adaptation/%.c src/abcc_adaptation/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H7xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc/anybus_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc/host_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src/host_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src/anybus_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/par" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/serial" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/spi" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/abcc-abp/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/src/abcc_adaptation" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/src/example_application" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-abcc_adaptation

clean-src-2f-abcc_adaptation:
	-$(RM) ./src/abcc_adaptation/abcc_hardware_abstraction.cyclo ./src/abcc_adaptation/abcc_hardware_abstraction.d ./src/abcc_adaptation/abcc_hardware_abstraction.o ./src/abcc_adaptation/abcc_hardware_abstraction.su

.PHONY: clean-src-2f-abcc_adaptation

