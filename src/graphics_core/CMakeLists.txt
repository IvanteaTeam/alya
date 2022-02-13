
file(GLOB_RECURSE headers ${ALYA_ROOT}/include/alya/graphics/core/*.hpp)

file(GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(alya_graphics_core STATIC ${headers} ${src})

target_include_directories(alya_graphics_core PUBLIC ${ALYA_ROOT}/include)

target_link_libraries(alya_graphics_core PUBLIC alya::ui::core)

target_link_libraries(alya_graphics_core PRIVATE d3d11 dxgi dxguid)

target_compile_features(alya_graphics_core PUBLIC cxx_std_20)

add_library(alya::graphics::core ALIAS alya_graphics_core)

