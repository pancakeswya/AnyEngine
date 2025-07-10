option(BUILD_OBJ_PARSER "Build obj format parser" ON)

if (BUILD_OBJ_PARSER)
    add_subdirectory(obj)
endif()