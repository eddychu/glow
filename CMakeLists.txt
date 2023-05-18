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

# add assimp
# set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Build assimp tools" FORCE)
# set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build assimp tests" FORCE)
# set(ASSIMP_INSTALL OFF CACHE BOOL "Generate installation target" FORCE)
# set(ASSIMP_NO_EXPORT ON CACHE BOOL "Disable assimp export" FORCE)
# # set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Build assimp with zlib" FORCE)
# add_subdirectory(extern/assimp)
# target_include_directories(glow PRIVATE extern/assimp/include)
# target_link_libraries(glow PRIVATE assimp)
# find_package(assimp REQUIRED)
# target_include_directories(glow PRIVATE ${ASSIMP_INCLUDE_DIRS})
# target_link_libraries(glow PRIVATE ${ASSIMP_LIBRARIES})

add_subdirectory(extern/tinygltf)
target_include_directories(glow PRIVATE extern/tinygltf)

add_subdirectory(extern/spdlog)
target_include_directories(glow PRIVATE extern/spdlog/include)
target_link_libraries(glow PRIVATE spdlog::spdlog)

file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
