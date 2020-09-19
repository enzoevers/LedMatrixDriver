add_library(Adafruit_GFX INTERFACE)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
message( ${PARENT_DIR} )
target_include_directories(Adafruit_GFX INTERFACE ${PARENT_DIR})