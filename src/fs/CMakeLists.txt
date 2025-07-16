
if (NOT TARGET SDL3::SDL3)
    find_package(SDL3 REQUIRED)
endif()

add_library(io STATIC)
target_sources(io PUBLIC
        error.h
        file.cc
        file.h
        path.h
        path.cc
)
target_link_libraries(io PUBLIC SDL3::SDL3)