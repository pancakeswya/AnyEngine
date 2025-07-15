include(${CMAKE_SOURCE_DIR}/cmake/plugins.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/shaders.cmake)

set(VK_VERSION "1.3.250")
set(VK_SDK_TAG "sdk-${VK_VERSION}")

configure_shaders(shaders shaders.in.cc shaders.cc)

get_target_property(GUI_INCLUDES gui INCLUDE_DIRECTORIES)
make_plugin(vk_api
        descriptor.h
        descriptor.cc
        sampler.h
        sampler.cc
        object.h
        object.cc
        command.h
        command.cc
        instance.h
        instance.cc
        device.h
        device.cc
        shader.h
        shader.cc
        framebuffer.h
        framebuffer.cc
        sync.h
        sync.cc
        buffer.h
        buffer.cc
        image.h
        image.cc
        handle.h
        swapchain.h
        swapchain.cc
        error.h
        surface.h
        surface.cc
        render.h
        render.cc
        queue.h
        queue.cc
        shaders.h
        shaders.cc
        gui.h
        gui.cc
        ${GUI_INCLUDES}/backends/imgui_impl_vulkan.cpp
        ${GUI_INCLUDES}/backends/imgui_impl_sdl3.cpp
        api_plugin.cc
        api.h
        api.cc
)

if (NOT TARGET Vulkan::Vulkan)
    find_package(Vulkan REQUIRED)
endif()

if (NOT TARGET SDL3::SDL3)
    find_package(SDL3 REQUIRED)
endif()

if (NOT TARGET shaderc_shared)
    include(FetchContent)

    set(SPIRV_CROSS_CLI OFF)
    set(SPIRV_CROSS_ENABLE_TESTS OFF)
    FetchContent_Declare(SPIRV-Cross
            GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git
            GIT_TAG ${VK_SDK_TAG}
    )

    set(SPIRV_HEADERS_SKIP_EXAMPLES ON)
    set(SPIRV_HEADERS_SKIP_INSTALL ON)
    FetchContent_Declare(SPIRV-Headers
            GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers.git
            GIT_TAG ${VK_SDK_TAG}
    )

    set(SKIP_SPIRV_TOOLS_INSTALL ON)
    set(SPIRV_SKIP_EXECUTABLES ON)
    set(SPIRV_SKIP_TESTS ON)
    FetchContent_Declare(SPIRV-Tools
            GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Tools.git
            GIT_TAG ${VK_SDK_TAG}
    )

    set(SKIP_GLSLANG_INSTALL ON)
    set(ENABLE_SPVREMAPPER OFF)
    set(ENABLE_GLSLANG_BINARIES OFF)
    set(ENABLE_GLSLANG_JS OFF)
    FetchContent_Declare(glslang
            GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
            GIT_TAG ${VK_SDK_TAG}
    )

    set(SHADERC_ENABLE_WGSL_OUTPUT OFF)
    set(SHADERC_SKIP_INSTALL ON)
    set(SHADERC_SKIP_TESTS ON)
    set(SHADERC_SKIP_EXAMPLES ON)
    set(SHADERC_SKIP_COPYRIGHT_CHECK ON)
    set(SHADERC_ENABLE_WERROR_COMPILE OFF)
    set(SHADERC_ENABLE_SHARED_CRT ON)
    FetchContent_Declare(shaderc
            GIT_REPOSITORY https://github.com/google/shaderc.git
            GIT_TAG v2024.0
    )

    FetchContent_MakeAvailable(SPIRV-Cross SPIRV-Headers SPIRV-Tools glslang shaderc)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND ANDROID)
    set(VVL_VERSION "${VK_VERSION}.0")
    set(VVL_NAME Vulkan-ValidationLayers-${VVL_VERSION}.zip)
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/${VVL_NAME})
        file(DOWNLOAD https://github.com/KhronosGroup/Vulkan-ValidationLayers/releases/download/sdk-${VVL_VERSION}/android-binaries-${VVL_VERSION}.zip ${CMAKE_BINARY_DIR}/${VVL_NAME})
    endif()
    if (NOT ${MOBILE_LIBS_DIR})
        file(MAKE_DIRECTORY ${MOBILE_LIBS_DIR})
    endif()
    file(ARCHIVE_EXTRACT INPUT ${CMAKE_BINARY_DIR}/${VVL_NAME} DESTINATION ${MOBILE_LIBS_DIR})
endif()

target_include_directories(vk_api PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(vk_api PUBLIC
        shaderc_shared
        SDL3::SDL3
        Vulkan::Vulkan
        render
        gui
)

if (APPLE)
    target_compile_definitions(vk_api PUBLIC VK_ENABLE_BETA_EXTENSIONS)
endif()