
add_library(sdl_vk_renderer STATIC)
target_sources(sdl_vk_renderer PRIVATE
        gui.h
        gui.cc
        error.h
        impl.h
        impl.cc
        texture_mapper.h
        texture_mapper.cc
        window.h
        window.cc
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

target_include_directories(sdl_vk_renderer PRIVATE ${IMGUI_DIR})
target_link_libraries(sdl_vk_renderer PUBLIC sdl_renderer sdl_gui sdl_image vk_render)