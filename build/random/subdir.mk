################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../random/Random.cpp 

CPP_DEPS += \
./random/Random.d 

OBJS += \
./random/Random.o 


# Each subdirectory must supply rules for building sources it contributes
random/%.o: ../random/%.cpp random/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O3 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-random

clean-random:
	-$(RM) ./random/Random.d ./random/Random.o

.PHONY: clean-random

