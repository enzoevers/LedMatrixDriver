add_library(Images INTERFACE)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
message( ${PARENT_DIR} )
target_include_directories(Images INTERFACE ${PARENT_DIR})