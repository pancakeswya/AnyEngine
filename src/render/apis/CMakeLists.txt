option(BUILD_VK_API "Build vulkan render api" ON)

if (BUILD_VK_API)
    add_subdirectory(vk)
endif()