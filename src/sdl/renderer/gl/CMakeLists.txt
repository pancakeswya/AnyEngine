
add_library(sdl_gl_renderer STATIC)
target_sources(sdl_gl_renderer PRIVATE
        gui.h
        gui.cc
        error.h
        impl.h
        impl.cc
        texture_mapper.h
        texture_mapper.cc
        window.h
        window.cc
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

if(USE_OPENGL_ES3)
    target_compile_definitions(sdl_gl_renderer PUBLIC USE_OPENGL_ES3)
endif()

target_include_directories(sdl_gl_renderer PRIVATE ${IMGUI_DIR})
target_link_libraries(sdl_gl_renderer PUBLIC sdl_renderer sdl_gui sdl_image gl_render)