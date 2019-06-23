################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/platform/bram_lib.c \
../src/platform/gpio_lib.c \
../src/platform/timer_lib.c 

OBJS += \
./src/platform/bram_lib.o \
./src/platform/gpio_lib.o \
./src/platform/timer_lib.o 

C_DEPS += \
./src/platform/bram_lib.d \
./src/platform/gpio_lib.d \
./src/platform/timer_lib.d 


# Each subdirectory must supply rules for building sources it contributes
src/platform/%.o: ../src/platform/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../flight_controller_bsp/flight_controller_flight_controller_mb/include -mlittle-endian -mno-xl-soft-div -mcpu=v10.0 -mno-xl-soft-mul -mxl-multiply-high -mhard-float -mxl-float-convert -mxl-float-sqrt -Wl,--no-relax -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


