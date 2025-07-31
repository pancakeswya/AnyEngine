include(${CMAKE_SOURCE_DIR}/cmake/plugins.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/shaders.cmake)

set(VK_VERSION "1.3.275.0")
set(VK_SDK_TAG "vulkan-sdk-${VK_VERSION}")

configure_shaders(shaders shaders.in.cc shaders.cc)
add_library(vk_render STATIC)
target_sources(vk_render PRIVATE
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
        render.h
        render.cc
        queue.h
        queue.cc
        shaders.h
        shaders.cc
        api.h
        api.cc
        window.h
        texture_mapper.h
)

if (IOS)
    function(download_moltenvk)
        set(MOLTENVK_PLATFORM "static/MoltenVK.xcframework/ios-arm64")
        if (CMAKE_OSX_SYSROOT MATCHES "iphonesimulator")
            set(MOLTENVK_PLATFORM "${MOLTENVK_PLATFORM}_x86_64-simulator")
        endif()
        set(MOLTENVK_DIR "${CMAKE_BINARY_DIR}/externals/MoltenVK")
        set(MOLTENVK_TAR "${CMAKE_BINARY_DIR}/externals/MoltenVK.tar")
        if (NOT EXISTS ${MOLTENVK_DIR})
            if (NOT EXISTS ${MOLTENVK_TAR})
                file(DOWNLOAD https://github.com/KhronosGroup/MoltenVK/releases/download/v1.3.0-rc1/MoltenVK-all.tar
                        ${MOLTENVK_TAR} SHOW_PROGRESS)
            endif()
            execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf "${MOLTENVK_TAR}"
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/externals")
        endif()
        set(MOLTENVK_INCLUDE_DIR "${MOLTENVK_DIR}/MoltenVK/include" PARENT_SCOPE)
        set(MOLTENVK_LIBRARY "${MOLTENVK_DIR}/MoltenVK/${MOLTENVK_PLATFORM}/libMoltenVK.a" PARENT_SCOPE)
    endfunction()
    download_moltenvk()

    target_include_directories(vk_render PUBLIC ${MOLTENVK_INCLUDE_DIR})
    target_link_libraries(vk_render PUBLIC Vulkan::Headers ${MOLTENVK_LIBRARY} "-framework Metal	       \
                                                                             -framework IOSurface 	   \
                                                                             -framework CoreGraphics   \
                                                                             -framework IOKit          \
                                                                             -framework UIKit 	       \
                                                                             -framework Foundation     \
                                                                             -framework QuartzCore     \
                                                                             -framework CoreFoundation")
else()
    if (APPLE)
        target_compile_definitions(vk_render PUBLIC USE_PORTABILITY)
    endif()
    if (NOT TARGET Vulkan::Vulkan)
        find_package(Vulkan REQUIRED)
    endif()
    target_link_libraries(vk_render PUBLIC Vulkan::Vulkan)
endif()

if (NOT TARGET shaderc)
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
            GIT_TAG v2024.3
    )
    FetchContent_MakeAvailable(SPIRV-Cross SPIRV-Headers SPIRV-Tools glslang shaderc)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND ANDROID)
    set(VVL_NAME Vulkan-ValidationLayers-${VK_VERSION}.zip)
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/${VVL_NAME})
        file(DOWNLOAD https://github.com/KhronosGroup/Vulkan-ValidationLayers/releases/download/${VK_SDK_TAG}/android-binaries-${VK_VERSION}.zip ${CMAKE_BINARY_DIR}/${VVL_NAME})
    endif()
    if (NOT ${MOBILE_LIBS_DIR})
        file(MAKE_DIRECTORY ${MOBILE_LIBS_DIR})
    endif()
    file(ARCHIVE_EXTRACT INPUT ${CMAKE_BINARY_DIR}/${VVL_NAME} DESTINATION ${MOBILE_LIBS_DIR})
endif()

target_link_libraries(vk_render PUBLIC shaderc render)

if (APPLE)
    target_compile_definitions(vk_render PUBLIC VK_ENABLE_BETA_EXTENSIONS)
endif()