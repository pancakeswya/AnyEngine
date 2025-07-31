include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY  https://github.com/g-truc/glm.git
        GIT_TAG         7882684a2cd69005fb57001c17a332899621e2be
)
FetchContent_MakeAvailable(glm)

add_library(render INTERFACE)
target_sources(render INTERFACE
        api.h
        error.h
        geometry.h
        geometry_transferer.h
        object.h
        object_parser.h
        texture.h
        api.h
        texture_mapper.h
)

add_subdirectory(apis)
add_subdirectory(parsers)

target_link_libraries(render INTERFACE glm)