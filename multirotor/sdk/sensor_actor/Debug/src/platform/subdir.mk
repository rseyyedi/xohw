################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/platform/bram_lib.c \
../src/platform/gam_lib.c \
../src/platform/gpio_lib.c \
../src/platform/iic_lib.c \
../src/platform/mcp3201_lib.c \
../src/platform/motor_lib.c \
../src/platform/mpl3115a2_lib.c \
../src/platform/mpu9150_lib.c \
../src/platform/ppm_rc.c \
../src/platform/spi_lib.c \
../src/platform/timer_lib.c \
../src/platform/uart_lib.c \
../src/platform/zynqtemp_lib.c 

OBJS += \
./src/platform/bram_lib.o \
./src/platform/gam_lib.o \
./src/platform/gpio_lib.o \
./src/platform/iic_lib.o \
./src/platform/mcp3201_lib.o \
./src/platform/motor_lib.o \
./src/platform/mpl3115a2_lib.o \
./src/platform/mpu9150_lib.o \
./src/platform/ppm_rc.o \
./src/platform/spi_lib.o \
./src/platform/timer_lib.o \
./src/platform/uart_lib.o \
./src/platform/zynqtemp_lib.o 

C_DEPS += \
./src/platform/bram_lib.d \
./src/platform/gam_lib.d \
./src/platform/gpio_lib.d \
./src/platform/iic_lib.d \
./src/platform/mcp3201_lib.d \
./src/platform/motor_lib.d \
./src/platform/mpl3115a2_lib.d \
./src/platform/mpu9150_lib.d \
./src/platform/ppm_rc.d \
./src/platform/spi_lib.d \
./src/platform/timer_lib.d \
./src/platform/uart_lib.d \
./src/platform/zynqtemp_lib.d 


# Each subdirectory must supply rules for building sources it contributes
src/platform/%.o: ../src/platform/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../sensor_actor_bsp/sensor_actor_sensor_actor_mb/include -mlittle-endian -mno-xl-soft-div -mcpu=v10.0 -mno-xl-soft-mul -mxl-multiply-high -mhard-float -mxl-float-convert -mxl-float-sqrt -Wl,--no-relax -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


