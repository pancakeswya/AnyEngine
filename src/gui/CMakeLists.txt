include(FetchContent)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.1)
FetchContent_GetProperties(imgui)
FetchContent_Populate(imgui)

add_library(imgui STATIC)
target_sources(imgui PRIVATE
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

add_library(gui STATIC)
target_sources(gui PRIVATE
        error.h
        renderer.h
        renderer.cc
)

set_target_properties(gui PROPERTIES GUI_IMGUI_DIR ${imgui_SOURCE_DIR})
target_include_directories(gui PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(gui PUBLIC imgui)