################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/altitude.c \
../src/attitude.c \
../src/controller.c \
../src/main.c \
../src/platform.c \
../src/remote.c \
../src/sensors.c 

OBJS += \
./src/altitude.o \
./src/attitude.o \
./src/controller.o \
./src/main.o \
./src/platform.o \
./src/remote.o \
./src/sensors.o 

C_DEPS += \
./src/altitude.d \
./src/attitude.d \
./src/controller.d \
./src/main.d \
./src/platform.d \
./src/remote.d \
./src/sensors.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../flight_controller_bsp/flight_controller_flight_controller_mb/include -mlittle-endian -mno-xl-soft-div -mcpu=v10.0 -mno-xl-soft-mul -mxl-multiply-high -mhard-float -mxl-float-convert -mxl-float-sqrt -Wl,--no-relax -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


