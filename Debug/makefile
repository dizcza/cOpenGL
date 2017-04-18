################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include Tests/subdir.mk
-include Src/subdir.mk
-include Drivers/linmath.h/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver_Additional/Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Drivers/OpenGL/Src/subdir.mk
-include Drivers/Fonts/subdir.mk
-include Drivers/Components/stmpe811/subdir.mk
-include Drivers/Components/l3gd20/subdir.mk
-include Drivers/Components/ili9341/subdir.mk
-include Drivers/BSP/subdir.mk
-include Demo/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: cOpenGL.elf

# Tool invocations
cOpenGL.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nosys.specs -u _printf_float -specs=nano.specs -specs=rdimon.specs -lc -lrdimon -T"../STM32F429ZITx_FLASH.ld" -Wl,-Map=output.map -Wl,--gc-sections -lm -o "cOpenGL.elf" @"objects.list" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "cOpenGL.elf" "cOpenGL.bin"
	arm-none-eabi-size "cOpenGL.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
