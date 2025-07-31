
add_library(sdl_gui STATIC)
target_sources(sdl_gui PRIVATE
        gui.h
        gui.cc
        ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
)

target_link_libraries(sdl_gui PUBLIC gui SDL3::SDL3)