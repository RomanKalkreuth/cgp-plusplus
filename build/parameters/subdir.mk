################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../parameters/Parameters.cpp 

CPP_DEPS += \
./parameters/Parameters.d 

OBJS += \
./parameters/Parameters.o 


# Each subdirectory must supply rules for building sources it contributes
parameters/%.o: ../parameters/%.cpp parameters/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O3 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-parameters

clean-parameters:
	-$(RM) ./parameters/Parameters.d ./parameters/Parameters.o

.PHONY: clean-parameters

