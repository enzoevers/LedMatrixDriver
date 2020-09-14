add_library(Adafruit_GFX INTERFACE)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
target_include_directories(Adafruit_GFX INTERFACE ${PARENT_DIR})