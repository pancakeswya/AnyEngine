add_library(render INTERFACE)
target_sources(render PUBLIC
        api.h
        api_plugin.h
        api_plugin_import.h
        error.h
        geometry.h
        geometry_transferer.h
        object.h
        object_parser.h
        object_parser_plugin.h
        object_parser_plugin_import.h
        texture.h
        texture_transferer.h
        gui.h
)

add_subdirectory(apis)
add_subdirectory(parsers)
add_subdirectory(mappers)

target_include_directories(render PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(render INTERFACE glm gui)
target_compile_definitions(render INTERFACE GLM_FORCE_RADIANS GLM_FORCE_DEPTH_ZERO_TO_ONE GLM_ENABLE_EXPERIMENTAL)