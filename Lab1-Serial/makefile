#
#             MEGN540 Mechatronics
#     Copyright (C) Andrew Petruska, 2020.
#
#  	apetruska [at] mines [dot] edu
#           www.mechanical.mines.edu
#
# --------------------------------------
#         Lab1-Serial Makefile.
# --------------------------------------
#
#  This make file was adopted from teh Lufa makefile for the
#  Dual Virtual Serial Example
#

# Run "make help" for target help.

# Name of the main c file and resulting executable
TARGET       = Lab1

# List C source files here. (C dependencies are automatically generated.)
SRC = $(TARGET).c       \
	$(MEGN_C_LIB_PATH)/SerialIO.c			\
	$(MEGN_C_LIB_PATH)/USB_Config/Descriptors.c       \
	$(LUFA_SRC_USB)

# List C++ source files here. (C dependencies are automatically generated.)
CPPSRC =

# Ohter Include Directories of importance
LUFA_DIR     = ../lufa
LUFA_PATH    = $(LUFA_DIR)/LUFA
MEGN_C_LIB_PATH = ../c_lib

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRAINCDIRS = $(LUFA_DIR) $(MEGN_C_LIB_PATH) $(MEGN_C_LIB_PATH)/USB_Config

#  Micro-Controller and USB Information
MCU          = atmega32u4
ARCH         = AVR8
BOARD        = USER
PORT	     = /dev/ttyZumoCarAVR
VID 		 = 0x1ffb #Dont Change These
PID 		 = 0x2300 #Dont Change These
F_CPU        = 16000000
F_USB        = $(F_CPU)

# Compiler Options
OPTIMIZATION = s
CFLAGS      += -g -Wall -mcall-prologues -mmcu=$(MCU) -Os
LDFLAGS	     = -Wl,-gc-sections -Wl,-relax
OBJDIR		 = BIN

# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
NM = avr-nm
FORMAT = ihex
AVRDUDE = /Applications/avrdude -C /Applications/avrdude.conf -B 1
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=c99

# Place -D or -U options here for C sources
CDEFS  = -DF_CPU=$(F_CPU)
CDEFS += -DF_USB=$(F_USB)
CDEFS += -DBOARD=BOARD_$(BOARD) -DARCH=ARCH_$(ARCH)
CDEFS += -DDEVICE_VID=$(VID)
CDEFS += -DDEVICE_PID=$(PID)

LUFA_OPTS = -DUSE_STATIC_OPTIONS=0 \
            -DUSB_DEVICE_ONLY \
            -DUSE_FLASH_DESCRIPTORS \
            -DFIXED_CONTROL_ENDPOINT_SIZE=8 \
            -DFIXED_NUM_CONFIGURATIONS=1 \
            -DINTERRUPT_CONTROL_ENDPOINT
CDEFS += $(LUFA_OPTS)

# Define all object files.
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(CPPSRC:%.cpp=$(OBJDIR)/%.o) $(ASRC:%.S=$(OBJDIR)/%.o)

# Define all listing files.
LST = $(SRC:%.c=$(OBJDIR)/%.lst) $(CPPSRC:%.cpp=$(OBJDIR)/%.lst) $(ASRC:%.S=$(OBJDIR)/%.lst)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d


# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(addprefix -I,$(EXTRAINCDIRS)) $(CFLAGS) $(GENDEPFLAGS) $(CDEFS)
ALL_CPPFLAGS = -mmcu=$(MCU) -I. $(addprefix -I,$(EXTRAINCDIRS)) -x c++ $(CPPFLAGS) $(GENDEPFLAGS)


# Include LUFA-specific make files and code
include $(LUFA_PATH)/Build/LUFA/lufa-sources.mk

# Default target.hex
all: $(TARGET).hex
elf: $(TARGET).elf
hex: $(TARGET).hex
sym: $(TARGET).sym
LIBNAME=lib$(TARGET).a
lib: $(LIBNAME)

# Display compiler version information.
gccversion :
	@$(CC) --version

program :
	avrdude -p $(MCU) -c avr109 -P $(PORT) -u -U flash:w:$(TARGET).hex

erase_program :
	avrdude -c avr109 -p atmega32U4 -P $(PORT) -u -e

reset_fuse:
	avrdude -c avr109 -p atmega32U4 -P $(PORT) -u -U efuse:w:0xcb:m -U hfuse:w:0xd8:m -U lfuse:w:0xFF:m

# Create final output files (.hex) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $(addprefix $(OBJDIR)/,$(notdir $<)) $(notdir $@)

## Create extended listing file from ELF output file.
#%.lss: %.elf
#	@echo
#	@echo $(MSG_EXTENDED_LISTING) $@
#	$(OBJDUMP) -h -S -z $< > $@

## Create a symbol table from ELF output file.
#%.sym: %.elf
#	@echo
#	@echo $(MSG_SYMBOL_TABLE) $@
#	$(NM) -n $< > $(addprefix $(OBJDIR)/,$(notdir $@))


## Create library from object files.
.SECONDARY : $(TARGET).a
.PRECIOUS : $(OBJ)
%.a: $(OBJ)
	@echo
	@echo $(MSG_CREATING_LIBRARY) $@
	$(AR) $@ $(addprefix $(OBJDIR)/,$(notdir $(OBJ)))


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $(addprefix $(OBJDIR)/,$(notdir $^)) --output $(OBJDIR)/$(notdir $@) $(LDFLAGS)


# Compile: create object files from C source files.
$(OBJDIR)/%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $(addprefix $(OBJDIR)/,$(notdir $@))


# Compile: create object files from C++ source files.
$(OBJDIR)/%.o : %.cpp
	@echo
	@echo $(MSG_COMPILING_CPP) $<
	$(CC) -c $(ALL_CPPFLAGS) $< -o $(addprefix $(OBJDIR)/,$(notdir $@))


# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $(addprefix $(OBJDIR)/,$(notdir $@))

# Compile: create assembler files from C++ source files.
%.s : %.cpp
	$(CC) -S $(ALL_CPPFLAGS) $< -o $(addprefix $(OBJDIR)/,$(notdir $@))


doxygen:
	@echo Generating Project Documentation \($(TARGET)\)...
	@doxygen Doxygen.conf
	@echo Documentation Generation Complete.

clean_doxygen:
	rm -rf Documentation

checksource:
	@for f in $(SRC) $(CPPSRC) $(ASRC); do \
		if [ -f $$f ]; then \
			echo "Found Source File: $$f" ; \
		else \
			echo "Source File Not Found: $$f" ; \
		fi; done 


# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)


# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Listing of phony targets.
.PHONY : all program gccversion elf hex doxygen clean          \
clean_list clean_doxygen checksource



clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/*.hex $(OBJDIR)/*.obj $(OBJDIR)/*.elf $(OBJDIR)/*.sym $(OBJDIR)/*.lss *.o *.hex *.obj *.hex *.elf *.sym *.lss


