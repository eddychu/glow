cmake_minimum_required(VERSION 3.20)
project(glow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c)

add_executable(glow ${SOURCES})

target_include_directories(glow PRIVATE src)
# add glad
add_subdirectory(extern/glad)
target_include_directories(glow PRIVATE extern/glad/include)
target_link_libraries(glow PRIVATE glad)

# add glfw
# configure glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target" FORCE)

add_subdirectory(extern/glfw)
# target_include_directories(glow PRIVATE extern/glfw/include)
target_link_libraries(glow PRIVATE glfw)

add_subdirectory(extern/glm)
target_include_directories(glow PRIVATE extern/glm)

# # add stb
target_include_directories(glow PRIVATE extern/stb)


add_subdirectory(extern/tinygltf)
target_include_directories(glow PRIVATE extern/tinygltf)

add_subdirectory(extern/spdlog)
target_include_directories(glow PRIVATE extern/spdlog/include)
target_link_libraries(glow PRIVATE spdlog::spdlog)


# find_package(SDL2 REQUIRED)
# target_include_directories(glow PRIVATE ${SDL2_INCLUDE_DIRS})
# target_link_libraries(glow PRIVATE ${SDL2_LIBRARIES})


file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
