################################################################ 
#
# sensor.c unit tests (based on gcc)
#
################################################################


################################################################
# target
################################################################
TARGET = test_sensor


################################################################
# build variables 
################################################################

# Debug build?
DEBUG = 1
# Optimization
OPT   = -Og


################################################################
# paths 
################################################################

# build folder
BUILD_DIR = build

# base directory
ROOT_DIR  = ../../../../..

# terminal directory
TERMINAL_DIR = $(ROOT_DIR)/terminal/rev4

# terminal mocked directory
MOCK_DIR = $(ROOT_DIR)/unit_tests/mock/terminal/rev4


################################################################
# source 
################################################################

# C sources
C_SOURCES =                         \
test_sensor.c                       \
$(TERMINAL_DIR)/mod/sensor/sensor.c \
$(ROOT_DIR)/Unity/src/unity.c


################################################################
# Compiler 
################################################################
CC = gcc


################################################################
# C flags 
################################################################

# C Includes
C_INCLUDES =                                       \
-I.                                                \
-I$(TERMINAL_DIR)/mod/sensor/                      \
-I$(ROOT_DIR)/Unity/src/                           \
-I$(TERMINAL_DIR)/config/Inc                       \
-I$(TERMINAL_DIR)/lib/\

# Object files
OBJECTS =                  \
$(BUILD_DIR)/test_sensor.o \
$(BUILD_DIR)/sensor.o      \
$(BUILD_DIR)/unity.o

CFLAGS = $(C_INCLUDES) $(OPT) -Wall -g

# build all 
all: $(BUILD_DIR)/$(TARGET)


################################################################
# Build the application 
################################################################

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $< -o $@
	mv $@ $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

$(BUILD_DIR):
	mkdir $@		

test:
	echo "$(BUILD_DIR)"


################################################################
# clean up 
################################################################

clean:
	-rm -fR $(BUILD_DIR)


################################################################
# EOF 
################################################################
