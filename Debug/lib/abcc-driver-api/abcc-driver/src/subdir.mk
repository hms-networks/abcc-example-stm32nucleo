################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/abcc-driver-api/abcc-driver/src/abcc_command_sequencer.c \
../lib/abcc-driver-api/abcc-driver/src/abcc_copy.c \
../lib/abcc-driver-api/abcc-driver/src/abcc_handler.c \
../lib/abcc-driver-api/abcc-driver/src/abcc_link.c \
../lib/abcc-driver-api/abcc-driver/src/abcc_log.c \
../lib/abcc-driver-api/abcc-driver/src/abcc_memory.c \
../lib/abcc-driver-api/abcc-driver/src/abcc_remap.c \
../lib/abcc-driver-api/abcc-driver/src/abcc_segmentation.c \
../lib/abcc-driver-api/abcc-driver/src/abcc_setup.c \
../lib/abcc-driver-api/abcc-driver/src/abcc_timer.c 

OBJS += \
./lib/abcc-driver-api/abcc-driver/src/abcc_command_sequencer.o \
./lib/abcc-driver-api/abcc-driver/src/abcc_copy.o \
./lib/abcc-driver-api/abcc-driver/src/abcc_handler.o \
./lib/abcc-driver-api/abcc-driver/src/abcc_link.o \
./lib/abcc-driver-api/abcc-driver/src/abcc_log.o \
./lib/abcc-driver-api/abcc-driver/src/abcc_memory.o \
./lib/abcc-driver-api/abcc-driver/src/abcc_remap.o \
./lib/abcc-driver-api/abcc-driver/src/abcc_segmentation.o \
./lib/abcc-driver-api/abcc-driver/src/abcc_setup.o \
./lib/abcc-driver-api/abcc-driver/src/abcc_timer.o 

C_DEPS += \
./lib/abcc-driver-api/abcc-driver/src/abcc_command_sequencer.d \
./lib/abcc-driver-api/abcc-driver/src/abcc_copy.d \
./lib/abcc-driver-api/abcc-driver/src/abcc_handler.d \
./lib/abcc-driver-api/abcc-driver/src/abcc_link.d \
./lib/abcc-driver-api/abcc-driver/src/abcc_log.d \
./lib/abcc-driver-api/abcc-driver/src/abcc_memory.d \
./lib/abcc-driver-api/abcc-driver/src/abcc_remap.d \
./lib/abcc-driver-api/abcc-driver/src/abcc_segmentation.d \
./lib/abcc-driver-api/abcc-driver/src/abcc_setup.d \
./lib/abcc-driver-api/abcc-driver/src/abcc_timer.d 


# Each subdirectory must supply rules for building sources it contributes
lib/abcc-driver-api/abcc-driver/src/%.o lib/abcc-driver-api/abcc-driver/src/%.su lib/abcc-driver-api/abcc-driver/src/%.cyclo: ../lib/abcc-driver-api/abcc-driver/src/%.c lib/abcc-driver-api/abcc-driver/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H7xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc/anybus_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/inc/host_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src/host_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/src/anybus_objects" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/par" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/serial" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/src/spi" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/lib/abcc-driver-api/abcc-driver/abcc-abp/inc" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/src/abcc_adaptation" -I"C:/Users/ajoh/unixhome/github/abcc-example-stm32nucleo/src/example_application" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib-2f-abcc-2d-driver-2d-api-2f-abcc-2d-driver-2f-src

clean-lib-2f-abcc-2d-driver-2d-api-2f-abcc-2d-driver-2f-src:
	-$(RM) ./lib/abcc-driver-api/abcc-driver/src/abcc_command_sequencer.cyclo ./lib/abcc-driver-api/abcc-driver/src/abcc_command_sequencer.d ./lib/abcc-driver-api/abcc-driver/src/abcc_command_sequencer.o ./lib/abcc-driver-api/abcc-driver/src/abcc_command_sequencer.su ./lib/abcc-driver-api/abcc-driver/src/abcc_copy.cyclo ./lib/abcc-driver-api/abcc-driver/src/abcc_copy.d ./lib/abcc-driver-api/abcc-driver/src/abcc_copy.o ./lib/abcc-driver-api/abcc-driver/src/abcc_copy.su ./lib/abcc-driver-api/abcc-driver/src/abcc_handler.cyclo ./lib/abcc-driver-api/abcc-driver/src/abcc_handler.d ./lib/abcc-driver-api/abcc-driver/src/abcc_handler.o ./lib/abcc-driver-api/abcc-driver/src/abcc_handler.su ./lib/abcc-driver-api/abcc-driver/src/abcc_link.cyclo ./lib/abcc-driver-api/abcc-driver/src/abcc_link.d ./lib/abcc-driver-api/abcc-driver/src/abcc_link.o ./lib/abcc-driver-api/abcc-driver/src/abcc_link.su ./lib/abcc-driver-api/abcc-driver/src/abcc_log.cyclo ./lib/abcc-driver-api/abcc-driver/src/abcc_log.d ./lib/abcc-driver-api/abcc-driver/src/abcc_log.o ./lib/abcc-driver-api/abcc-driver/src/abcc_log.su ./lib/abcc-driver-api/abcc-driver/src/abcc_memory.cyclo ./lib/abcc-driver-api/abcc-driver/src/abcc_memory.d ./lib/abcc-driver-api/abcc-driver/src/abcc_memory.o ./lib/abcc-driver-api/abcc-driver/src/abcc_memory.su ./lib/abcc-driver-api/abcc-driver/src/abcc_remap.cyclo ./lib/abcc-driver-api/abcc-driver/src/abcc_remap.d ./lib/abcc-driver-api/abcc-driver/src/abcc_remap.o ./lib/abcc-driver-api/abcc-driver/src/abcc_remap.su ./lib/abcc-driver-api/abcc-driver/src/abcc_segmentation.cyclo ./lib/abcc-driver-api/abcc-driver/src/abcc_segmentation.d ./lib/abcc-driver-api/abcc-driver/src/abcc_segmentation.o ./lib/abcc-driver-api/abcc-driver/src/abcc_segmentation.su ./lib/abcc-driver-api/abcc-driver/src/abcc_setup.cyclo ./lib/abcc-driver-api/abcc-driver/src/abcc_setup.d ./lib/abcc-driver-api/abcc-driver/src/abcc_setup.o ./lib/abcc-driver-api/abcc-driver/src/abcc_setup.su ./lib/abcc-driver-api/abcc-driver/src/abcc_timer.cyclo ./lib/abcc-driver-api/abcc-driver/src/abcc_timer.d ./lib/abcc-driver-api/abcc-driver/src/abcc_timer.o ./lib/abcc-driver-api/abcc-driver/src/abcc_timer.su

.PHONY: clean-lib-2f-abcc-2d-driver-2d-api-2f-abcc-2d-driver-2f-src

