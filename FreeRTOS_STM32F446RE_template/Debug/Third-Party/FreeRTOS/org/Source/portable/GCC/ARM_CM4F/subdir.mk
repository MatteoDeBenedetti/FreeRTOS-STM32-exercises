################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F/%.o: ../Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -DSTM32F446xx -DUSE_STDPERIPH_DRIVER -I"D:/Users/Matteo/Documents/RTOS_workspace/FreeRTOS_STM32F446RE_template/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"D:/Users/Matteo/Documents/RTOS_workspace/FreeRTOS_STM32F446RE_template/StdPeriph_Driver/inc" -I"D:/Users/Matteo/Documents/RTOS_workspace/FreeRTOS_STM32F446RE_template/Config" -I"D:/Users/Matteo/Documents/RTOS_workspace/FreeRTOS_STM32F446RE_template/Third-Party/FreeRTOS/org/Source/include" -I"D:/Users/Matteo/Documents/RTOS_workspace/FreeRTOS_STM32F446RE_template/inc" -I"D:/Users/Matteo/Documents/RTOS_workspace/FreeRTOS_STM32F446RE_template/CMSIS/device" -I"D:/Users/Matteo/Documents/RTOS_workspace/FreeRTOS_STM32F446RE_template/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


