include(FetchContent)

if (IOS)
    set(SDL_SHARED OFF)
    set(SDL_STATIC ON)
endif()
FetchContent_Declare(
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG 2dad6534b841c449100db3a5647fa867ce4db199
)

set(SDLMIXER_MIDI_NATIVE OFF)
set(SDLMIXER_GME OFF)
set(SDLMIXER_WAVPACK OFF)
set(SDLMIXER_MOD OFF)
set(SDLMIXER_OPUS OFF)
set(SDLMIXER_VENDORED ON)
FetchContent_Declare(
        SDL_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG 3d55061a1ea31052d23e74256f2ba2cae8586234
)

FetchContent_MakeAvailable(SDL SDL_mixer)

include_directories(.)

option(USE_VK_API "Use vulkan api for render" ON)
if (EMSCRIPTEN)
    set(USE_VK_API OFF)
endif()

if (NOT USE_VK_API)
    set(USE_OPENGL_ES3 OFF)
    if (ANDROID OR EMSCRIPTEN OR IOS)
        set(USE_OPENGL_ES3 ON)
    endif()
endif()

add_subdirectory(resource)
add_subdirectory(base)
add_subdirectory(fs)
add_subdirectory(render)
add_subdirectory(gui)
add_subdirectory(sdl)

set(EXECUTABLE_NAME ${PROJECT_NAME})

if (ANDROID)
    set(EXECUTABLE_NAME main)
    add_library(${EXECUTABLE_NAME} SHARED)
else()
    add_executable(${EXECUTABLE_NAME})
endif()
target_sources(${EXECUTABLE_NAME} PRIVATE app.h app.cc main.cc)
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_20)

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
    target_link_libraries(${EXECUTABLE_NAME} PUBLIC -sMAX_WEBGL_VERSION=2 -sMIN_WEBGL_VERSION=2 -sALLOW_MEMORY_GROWTH=1)
endif()

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC
        SDL_MAIN_USE_CALLBACKS
        GLM_FORCE_RADIANS
        GLM_FORCE_DEPTH_ZERO_TO_ONE
        GLM_ENABLE_EXPERIMENTAL
)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
        SDL3::SDL3
        SDL3_mixer::SDL3_mixer
        sdl_backend
        resource
)

if(EMSCRIPTEN)
    target_link_libraries(${PROJECT_NAME} PRIVATE "--preload-file \"${CMAKE_SOURCE_DIR}/resources/obj/@obj\"")
else()
    file(GLOB_RECURSE RESOURCES "${CMAKE_SOURCE_DIR}/resources/obj/*")
    if(APPLE)
        set(APP_SOURCES ${CMAKE_SOURCE_DIR}/resources/icons/logo.png)
        set(BUNDLE_FILES "${CMAKE_SOURCE_DIR}/resources/icons/logo.png")
        if(IOS)
            list(APPEND APP_SOURCES ${CMAKE_SOURCE_DIR}/resources/apple/iosLaunchScreen.storyboard)
            set(BUNDLE_FILES "${CMAKE_SOURCE_DIR}/resources/apple/iosLaunchScreen.storyboard;${BUNDLE_FILES}")
        endif()
        target_sources(${EXECUTABLE_NAME} PRIVATE ${APP_SOURCES})
        foreach(RESOURCE ${RESOURCES})
            file(RELATIVE_PATH RELPATH "${CMAKE_SOURCE_DIR}/resources" "${RESOURCE}")
            get_filename_component(RELPATH "${RELPATH}" DIRECTORY)
            target_sources(${EXECUTABLE_NAME} PRIVATE ${RESOURCE})
            set_property(SOURCE ${RESOURCE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${RELPATH}")
            set_property(SOURCE ${RESOURCE} PROPERTY HEADER_FILE_ONLY ON)
        endforeach()
        set_target_properties(${EXECUTABLE_NAME} PROPERTIES
                MACOSX_BUNDLE TRUE
                MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/resources/apple/Info.plist.in"

                XCODE_GENERATE_SCHEME TRUE

                XCODE_ATTRIBUTE_BUNDLE_IDENTIFIER "com.pancakeswya.${EXECUTABLE_NAME}"
                XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.pancakeswya.${EXECUTABLE_NAME}"
                XCODE_ATTRIBUTE_CURRENTYEAR "${CURRENTYEAR}"
                RESOURCE "${BUNDLE_FILES}"
        )
        if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
            install(TARGETS ${PROJECT_NAME}
                    BUNDLE DESTINATION ./install COMPONENT Runtime
                    RUNTIME DESTINATION ./install/bin COMPONENT Runtime
            )

            set(DEP_DIR "${CMAKE_BINARY_DIR}")
            INSTALL(CODE
                    "include(BundleUtilities)
        fixup_bundle(\"$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>\" \"\" \"${DEP_DIR}\")
        "
            )
            set(CPACK_GENERATOR "DragNDrop")
            include(CPack)
        endif()
    else()
        if (ANDROID)
            if (NOT EXISTS ${MOBILE_ASSETS_DIR})
                file(MAKE_DIRECTORY ${MOBILE_ASSETS_DIR})
            endif ()
            set(OUTDIR ${MOBILE_ASSETS_DIR})
        else()
            set(OUTDIR "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
        endif()

        foreach(RESOURCE ${RESOURCES})
            file(RELATIVE_PATH RELPATH "${CMAKE_SOURCE_DIR}/resources" ${RESOURCE})
            add_custom_command(POST_BUILD
                    TARGET "${EXECUTABLE_NAME}"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${RESOURCE}" "${OUTDIR}/${RELPATH}"
                    DEPENDS "${RESOURCE}"
            )
        endforeach()
    endif()
endif()
