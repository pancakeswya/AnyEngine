
add_library(sdl_backend STATIC)

target_sources(sdl_backend PRIVATE
        error.h
        render3d.h
        render3d.cc
)

add_subdirectory(image)
add_subdirectory(gui)
add_subdirectory(renderer)

target_link_libraries(sdl_backend PUBLIC SDL3::SDL3 obj_parser)
if(USE_VK_API)
    target_link_libraries(sdl_backend PUBLIC sdl_vk_renderer)
    target_compile_definitions(sdl_backend PUBLIC USE_VK_API)
else()
    target_link_libraries(sdl_backend PUBLIC sdl_gl_renderer)
endif()