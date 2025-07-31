include(FetchContent)
include(${CMAKE_SOURCE_DIR}/cmake/shaders.cmake)

configure_shaders(shaders shaders.in.cc shaders.cc)

add_library(gl_render STATIC)
target_sources(gl_render PRIVATE
        api.h
        api.cc
        check.cc
        check.h
        handle.h
        loader.h
        error.h
        error.cc
        object.h
        object.cc
        buffer.h
        texture.h
        texture.cc
        shader.h
        shader.cc
        shaders.h
        shaders.cc
        texture_mapper.h
        window.h
)

if (USE_OPENGL_ES3)
    set(GLAD_GL_ARG gles2:core=3.0)
    target_compile_definitions(gl_render PUBLIC USE_OPENGL_ES3)
    if (ANDROID)
        target_compile_definitions(gl_render PUBLIC USE_BUFFER_MAP)
        target_link_libraries(gl_render PUBLIC GLESv3)
    elseif(IOS)
        target_link_libraries(
                gl_render PUBLIC "-framework OpenGLES	\
					-framework UIKit 		\
					-framework QuartzCore  	\
					-framework CoreVideo  	\
					-framework CoreGraphics \
					-framework AVFoundation \
					-framework CoreMedia"
        )
    endif()
else()
    find_package(OpenGL REQUIRED)
    set(GLAD_GL_ARG gl:core=3.3)
    target_compile_definitions(gl_render PUBLIC USE_BUFFER_MAP)
    target_link_libraries(gl_render PUBLIC OpenGL::GL)
endif()

if (NOT TARGET glad)
    FetchContent_Declare(
            glad
            GIT_REPOSITORY https://github.com/Dav1dde/glad.git
            GIT_TAG v2.0.8
            SOURCE_SUBDIR cmake
    )
    FetchContent_MakeAvailable(glad)
    glad_add_library(glad STATIC API ${GLAD_GL_ARG} LOCATION ${CMAKE_BINARY_DIR}/glad-build/${TARGET})
endif()

target_include_directories(gl_render PUBLIC ${IMGUI_DIR})
target_compile_definitions(gl_render PUBLIC USE_VAO)
target_link_libraries(gl_render PUBLIC glad render)