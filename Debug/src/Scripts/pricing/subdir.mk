################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Scripts/pricing/Base.cpp \
../src/Scripts/pricing/Pricer.cpp \
../src/Scripts/pricing/PricerABRP.cpp \
../src/Scripts/pricing/Pricermain.cpp 

OBJS += \
./src/Scripts/pricing/Base.o \
./src/Scripts/pricing/Pricer.o \
./src/Scripts/pricing/PricerABRP.o \
./src/Scripts/pricing/Pricermain.o 

CPP_DEPS += \
./src/Scripts/pricing/Base.d \
./src/Scripts/pricing/Pricer.d \
./src/Scripts/pricing/PricerABRP.d \
./src/Scripts/pricing/Pricermain.d 


# Each subdirectory must supply rules for building sources it contributes
src/Scripts/pricing/%.o: ../src/Scripts/pricing/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


