######################################################################
#  ARM Cortex-M3 Example Makefile
#
#  Main Build Process: 
#      1. Compile program.c and startup.c sourcefiles to .o files
#      2. Run the linker to generate the merged .elf file
#      3. Copy the .elf file into .bin format for the MCU
#      4. Flash the MCU with the .bin file via the SWD interface and
#         the st-flash utility  
#  
#  Additional Build Features:
#      1. pre-process: generate the result of executing the 
#         pre-processor directives
#      2. assembly: generate the assembly from the C source code
#      3. printSymbols: print all the symbols and corresponding 
#         memory addresses produced by the linker
######################################################################

# Program Name
PROGRAM = testSWD

# Library directory
LIBDIR = ../../lib/

# Cross Compiler
CC = arm-none-eabi-gcc

# Assembler 
ASM = arm-none-eabi-as

# Processor 
MPU = cortex-m4

# Compiler Arguments
# Use hardware FPU, Optimize for space, add debuging functionality, 
# use GNU dialiect of ISO C11, execute thumb instructions, 
# generate warnings during compilation
CCARG = -mcpu=$(MPU) -mfloat-abi=hard -O0 -std=gnu11 -mthumb -Wall

# Linker arguments
LLARG = -mcpu=$(MPU) -nostdlib -g -Wl,-Map=testSWD.map

## Main Build
all: testSWD.o startup.o testSWD.elf testSWD.bin

testSWD.bin: testSWD.elf
	arm-none-eabi-objcopy -O binary testSWD.elf testSWD.bin

testSWD.elf: testSWD.o startup.o
	$(CC) $(LLARG) -T testSWD.ld -o testSWD.elf testSWD.o startup.o

startup.o: startup.c
	$(CC) $(CCARG) -c -o startup.o startup.c

testSWD.o: testSWD.c
	$(CC) $(CCARG) -c -o testSWD.o testSWD.c
