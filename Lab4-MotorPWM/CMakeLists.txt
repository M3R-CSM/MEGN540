file(GLOB LAB_SRC_FILES "*.c")

# Create one target
add_avr_executable(Lab4  ${LAB_SRC_FILES} )# ${MEG540_C_LIB_SRC_FILES} )

# LINK Agains LUFA Libarary 
avr_target_link_libraries(Lab4 LUFA_USB MEGN540)
