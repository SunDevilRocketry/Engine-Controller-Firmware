######################################################################
#  ARM Cortex-M4 Example Makefile
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

#########################################################################
## Main Build
#########################################################################
all: testSWD.o startup.o testSWD.elf testSWD.bin

testSWD.bin: testSWD.elf
	arm-none-eabi-objcopy -O binary testSWD.elf testSWD.bin

testSWD.elf: testSWD.o startup.o
	$(CC) $(LLARG) -T testSWD.ld -o testSWD.elf testSWD.o startup.o

startup.o: startup.c
	$(CC) $(CCARG) -c -o startup.o startup.c

testSWD.o: testSWD.c
	$(CC) $(CCARG) -c -o testSWD.o testSWD.c

#########################################################################
## Flash to MCU
#########################################################################
flash:
	st-flash write testSWD.bin 0x08000000

#########################################################################
## Additional Features
#########################################################################

# Generate the ARM Assembly language
assembly: testSWD.c
	$(CC) $(CCARG) -S -o testSWD.asm testSWD.c

# Generate the output of the preprocessor
pre-process: testSWD.c
	$(CC) $(CCARG) -E -o testSWD.i testSWD.c

# Print all the symbol produced by the linker
printSymbols: testSWD.elf
	arm-none-eabi-nm testSWD.elf

# Remove all build files
clean:
	rm *.bin *.o *.elf *.map *.asm *.i
# End
