
add_library(sdl_renderer INTERFACE renderer.h)
target_link_libraries(sdl_renderer INTERFACE SDL3::SDL3 render)

if (USE_VK_API)
    add_subdirectory(vk)
else()
    add_subdirectory(gl)
endif()