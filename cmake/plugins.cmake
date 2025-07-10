include_guard()

function(make_plugin NAME)
    add_library(${NAME} SHARED)
    target_sources(${NAME} PUBLIC ${ARGN})
    target_compile_definitions(${NAME} PUBLIC PLUGIN_EXPORT)
    set_target_properties(${NAME} PROPERTIES PREFIX "lib")
    set_property(TARGET ${NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
endfunction()