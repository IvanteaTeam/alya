cmake_minimum_required(VERSION 3.20)

project(gl_dev)

set(vs "shaders/3/a.hlsl;shaders/3/d1.hlsl;shaders/3/m1.hlsl")
set(ps "shaders/3/b.hlsl;shaders/3/d2.hlsl;shaders/3/m2.hlsl")

set_source_files_properties(${vs} PROPERTIES VS_SHADER_TYPE Vertex VS_SHADER_MODEL 4.0)
set_source_files_properties(${ps} PROPERTIES VS_SHADER_TYPE Pixel VS_SHADER_MODEL 4.0)

add_executable(gl_dev main.cpp stb_image.cpp scene.hpp help.cpp help.hpp model.hpp model.cpp ${vs} ${ps} "shaders/3/c.h")

find_package(assimp QUIET REQUIRED)

target_link_libraries(gl_dev 
PRIVATE 
alya::graphics::core
alya::resource
assimp::assimp
)


target_compile_features(gl_dev PRIVATE cxx_std_20)


target_include_directories(gl_dev PRIVATE
 ${assimp_SOURCE_DIR}/include/
 ${assimp_BINARY_DIR}/include/
 ${stb_SOURCE_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}/pfr
)
