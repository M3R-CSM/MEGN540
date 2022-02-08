# I target a recent cmake, it shouldn't be a problem on a dev machine
cmake_minimum_required(VERSION 2.8)

## AVR Chip Configuration
# 16Mhz, this should match the crystal on your board,
set(F_CPU 16000000UL)

##########################################################################
# tools to beused for programming the AVR
##########################################################################
set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER avr109)
set(AVR_UPLOADTOOL_PORT /dev/ttyZumoCarAVR)

##########################################################################
# AVR and fuses needs to be set
##########################################################################
set(AVR_MCU atmega32u4)
set(AVR_H_FUSE 0xd8)
set(AVR_L_FUSE 0xFF)
set(AVR_E_FUSE 0xcb)
set(MCU_SPEED ${F_CPU})

include(generic-gcc-avr.cmake)

## USB Information for LUFA Library
set(MCU ${AVR_MCU})
set(ARCH AVR8)
set(BOARD USER)
set(VID 0x1ffb)
set(PID 0x2300)
set(F_USB ${F_CPU})


# Project name
project("MEGN 540 Tank Control Lab")

##########################################################################
# status messages
##########################################################################
message(STATUS "Current uploadtool is: ${AVR_UPLOADTOOL}")
message(STATUS "Current programmer is: ${AVR_PROGRAMMER}")
message(STATUS "Current upload port is: ${AVR_UPLOADTOOL_PORT}")
message(STATUS "Current uploadtool options are: ${AVR_UPLOADTOOL_OPTIONS}")
message(STATUS "Current MCU is set to: ${AVR_MCU}")
message(STATUS "Current MCU speed is set to: ${MCU_SPEED}")
message(STATUS "Current H_FUSE is set to: ${AVR_H_FUSE}")
message(STATUS "Current L_FUSE is set to: ${AVR_L_FUSE}")

##########################################################################
# set build type
##########################################################################
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)


##########################################################################
# some cmake cross-compile necessities
##########################################################################
if(DEFINED ENV{AVR_FIND_ROOT_PATH})
    set(CMAKE_FIND_ROOT_PATH $ENV{AVR_FIND_ROOT_PATH})
else(DEFINED ENV{AVR_FIND_ROOT_PATH})
    if(EXISTS "/opt/local/avr")
      set(CMAKE_FIND_ROOT_PATH "/opt/local/avr")
    elseif(EXISTS "/usr/avr")
      set(CMAKE_FIND_ROOT_PATH "/usr/avr")
    elseif(EXISTS "/usr/lib/avr")
      set(CMAKE_FIND_ROOT_PATH "/usr/lib/avr")
    elseif(EXISTS "/usr/local/CrossPack-AVR")
      set(CMAKE_FIND_ROOT_PATH "/usr/local/CrossPack-AVR")
    else(EXISTS "/opt/local/avr")
      message(FATAL_ERROR "Please set AVR_FIND_ROOT_PATH in your environment.")
    endif(EXISTS "/opt/local/avr")
endif(DEFINED ENV{AVR_FIND_ROOT_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# not added automatically, since CMAKE_SYSTEM_NAME is "generic"
set(CMAKE_SYSTEM_INCLUDE_PATH "${CMAKE_FIND_ROOT_PATH}/include")
set(CMAKE_SYSTEM_LIBRARY_PATH "${CMAKE_FIND_ROOT_PATH}/lib")

##########################################################################
# status messages for generating
##########################################################################
message(STATUS "Set CMAKE_FIND_ROOT_PATH to ${CMAKE_FIND_ROOT_PATH}")
message(STATUS "Set CMAKE_SYSTEM_INCLUDE_PATH to ${CMAKE_SYSTEM_INCLUDE_PATH}")
message(STATUS "Set CMAKE_SYSTEM_LIBRARY_PATH to ${CMAKE_SYSTEM_LIBRARY_PATH}")

##########################################################################
# Find Libraries
##########################################################################

find_library(C_LIB c)
message(STATUS "Found avr-libc at: ${C_LIB}")

##########################################################################
# set compiler options for build types
##########################################################################
if(CMAKE_BUILD_TYPE MATCHES Release)
   set(CMAKE_C_FLAGS_RELEASE "-Os")
   set(CMAKE_CXX_FLAGS_RELEASE "-Os")
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
   set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
   set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
endif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

if(CMAKE_BUILD_TYPE MATCHES Debug)
   set(CMAKE_C_FLAGS_DEBUG "-O0 -save-temps -g -gdwarf-3 -gstrict-dwarf")
   set(CMAKE_CXX_FLAGS_DEBUG "-O0 -save-temps -g -gdwarf-3 -gstrict-dwarf")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

##########################################################################
# compiler options for all build types
##########################################################################
add_definitions(
  # Define Clock Speed as F_CPU 
  -DF_CPU=${MCU_SPEED}

  # Compile Flags (https://linux.die.net/man/1/avr-gcc)
  -fpack-struct # this signals the compiler not to add padding to structures. This fine becasue were running on an 8bit architecture anyway
  -fshort-enums # choose enum type based on max enum value required. This will likely keep them as 8-bit for our purposes.
  -Wall    # this turns on all warnings
  -Werror  # this turns warnings in to errors.
  -funsigned-char # ensures char is treated as unsigned during math 
  -funsigned-bitfields # ensures bitfields are treated as unsigned during math 
  -ffunction-sections  #places each funciton into its own section allowing for better elf optimizations
  -c # dont do linking (that will be done later)
  -std=gnu99

  # LUFA DEFNITIONS
  -DF_USB=${F_USB}
  -DBOARD=BOARD_${BOARD} 
  -DARCH=ARCH_${ARCH}
  -DDEVICE_VID=${VID}
  -DDEVICE_PID=${PID}
  -DUSE_STATIC_OPTIONS=0
  -DUSB_DEVICE_ONLY
  -DUSE_FLASH_DESCRIPTORS
  -DFIXED_CONTROL_ENDPOINT_SIZE=8
  -DFIXED_NUM_CONFIGURATIONS=1
  -DINTERRUPT_CONTROL_ENDPOINT
)

set(MEG540_C_LIB_PATH ${CMAKE_CURRENT_LIST_DIR}/c_lib)
set(LUFA_DIR ${CMAKE_CURRENT_LIST_DIR}/lufa)
set(LUFA_PATH ${LUFA_DIR}/LUFA)

# Find and add lufa subdirs for header files
include_directories(${LUFA_DIR} ${MEG540_C_LIB_PATH} ${MEG540_C_LIB_PATH}/USB_Config)

## ADD LUFA LIBRARY
add_avr_library( LUFA_USB 
   ## Project Specific LUFA Descriptors
   ${MEG540_C_LIB_PATH}/USB_Config/Descriptors.c

   ## Lufa Core Code for USB Interface
   ${LUFA_PATH}/Drivers/USB/Core/${ARCH}/USBController_${ARCH}.c
   ${LUFA_PATH}/Drivers/USB/Core/${ARCH}/USBInterrupt_${ARCH}.c
   ${LUFA_PATH}/Drivers/USB/Core/ConfigDescriptors.c
   ${LUFA_PATH}/Drivers/USB/Core/Events.c
   ${LUFA_PATH}/Drivers/USB/Core/USBTask.c
   ${LUFA_PATH}/Drivers/USB/Class/Common/HIDParser.c 

   ${LUFA_PATH}/Drivers/USB/Core/${ARCH}/Host_${ARCH}.c
   ${LUFA_PATH}/Drivers/USB/Core/${ARCH}/Pipe_${ARCH}.c
   ${LUFA_PATH}/Drivers/USB/Core/${ARCH}/PipeStream_${ARCH}.c
   ${LUFA_PATH}/Drivers/USB/Core/HostStandardReq.c

   ${LUFA_PATH}/Drivers/USB/Core/${ARCH}/Device_${ARCH}.c
   ${LUFA_PATH}/Drivers/USB/Core/${ARCH}/Endpoint_${ARCH}.c
   ${LUFA_PATH}/Drivers/USB/Core/${ARCH}/EndpointStream_${ARCH}.c
   ${LUFA_PATH}/Drivers/USB/Core/DeviceStandardReq.c
)

## ADD MEGN540 Library

file(GLOB MEG540_C_LIB_SRC_FILES "${MEG540_C_LIB_PATH}/*.c") # Load all files in src folder
add_avr_library( MEGN540 ${MEG540_C_LIB_SRC_FILES} )
avr_target_link_libraries(MEGN540 LUFA_USB)

# Add Lab Subdirectories
add_subdirectory(Lab0-Blink)
add_subdirectory(Lab1-Serial)
add_subdirectory(Lab2-Timing)
add_subdirectory(Lab3-Encoder)
add_subdirectory(Lab4-MotorPWM)
add_subdirectory(Lab5-Control)

