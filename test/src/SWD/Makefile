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

