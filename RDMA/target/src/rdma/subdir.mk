################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/rdma/rdma.c \
../src/rdma/rdma_common.c 

OBJS += \
./src/rdma/rdma.o \
./src/rdma/rdma_common.o


# Each subdirectory must supply rules for building sources it contributes
src/rdma/%.o: ../src/rdma/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc-4.8 -fPIC -rdynamic -std=gnu11 -DDEBUG=$(DEBUGOPT) -I"$(ROOT_DIR)/../.local/include" -O0 -g3 -Wall -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


