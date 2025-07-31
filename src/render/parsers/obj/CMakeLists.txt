include(${CMAKE_SOURCE_DIR}/cmake/plugins.cmake)
include(FetchContent)

add_library(obj_parser STATIC)
target_sources(obj_parser PRIVATE
        error.h
        types.h
        object_parser.h
        object_parser.cc
        transferer.h
        transferer.cc
)

set(EARCUT_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(EARCUT_BUILD_BENCH OFF CACHE BOOL "" FORCE)
set(EARCUT_BUILD_VIZ OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        earcut
        GIT_REPOSITORY  https://github.com/mapbox/earcut.hpp.git
        GIT_TAG         7fa7aa30183849e988ae79ab2eef19f9ae97acf4
)

FetchContent_MakeAvailable(earcut)

target_link_libraries(obj_parser PUBLIC earcut_hpp::earcut_hpp io render)