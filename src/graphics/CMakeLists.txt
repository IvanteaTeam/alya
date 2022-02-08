
file(GLOB_RECURSE headers ${ALYA_ROOT}/include/alya/graphics/*.hpp)

file(GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(alya_graphics STATIC ${headers} ${src})

target_include_directories(alya_graphics PUBLIC ${ALYA_ROOT}/include)

target_link_libraries(alya_graphics PUBLIC alya::ui::core alya::resource)

target_link_libraries(alya_graphics PRIVATE d3d11 dxgi dxguid)

target_compile_features(alya_graphics PUBLIC cxx_std_20)

add_library(alya::graphics ALIAS alya_graphics)

