
file(GLOB_RECURSE headers ${ALYA_ROOT}/include/alya/async/*.hpp)

file(GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(alya_async STATIC ${headers} ${src})

target_include_directories(alya_async PUBLIC ${ALYA_ROOT}/include)

target_link_libraries(alya_async PUBLIC alya::utility CONAN_PKG::boost)

target_compile_features(alya_async PUBLIC cxx_std_20)


add_library(alya::async ALIAS alya_async)
