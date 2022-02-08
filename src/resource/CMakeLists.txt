

file(GLOB_RECURSE headers ${ALYA_ROOT}/include/alya/resource/*.hpp)

file(GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(alya_resource STATIC ${headers} ${src})

target_include_directories(alya_resource PUBLIC ${ALYA_ROOT}/include)

target_link_libraries(alya_resource PUBLIC alya::async CONAN_PKG::stb)

target_compile_features(alya_resource PUBLIC cxx_std_20)

add_library(alya::resource ALIAS alya_resource)

