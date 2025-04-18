################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/example_application/abcc_network_data_parameters.c \
../src/example_application/implemented_callback_functions.c 

OBJS += \
./src/example_application/abcc_network_data_parameters.o \
./src/example_application/implemented_callback_functions.o 

C_DEPS += \
./src/example_application/abcc_network_data_parameters.d \
./src/example_application/implemented_callback_functions.d 


# Each subdirectory must supply rules for building sources it contributes
src/example_application/%.o src/example_application/%.su src/example_application/%.cyclo: ../src/example_application/%.c src/example_application/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H7xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc/anybus_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc/host_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src/host_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src/anybus_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/par" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/serial" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/spi" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/abcc-abp/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/src/abcc_adaptation" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/src/example_application" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-example_application

clean-src-2f-example_application:
	-$(RM) ./src/example_application/abcc_network_data_parameters.cyclo ./src/example_application/abcc_network_data_parameters.d ./src/example_application/abcc_network_data_parameters.o ./src/example_application/abcc_network_data_parameters.su ./src/example_application/implemented_callback_functions.cyclo ./src/example_application/implemented_callback_functions.d ./src/example_application/implemented_callback_functions.o ./src/example_application/implemented_callback_functions.su

.PHONY: clean-src-2f-example_application

