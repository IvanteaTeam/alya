
file(GLOB headers ${ALYA_ROOT}/include/alya/audio/*.hpp)

file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(alya_audio STATIC ${headers} ${src})

target_include_directories(alya_audio PUBLIC ${ALYA_ROOT}/include)

target_link_libraries(alya_audio PUBLIC alya::utility alya::resource alya::audio::core)

target_link_libraries(alya_audio PRIVATE CONAN_PKG::libsndfile)

target_compile_features(alya_audio PUBLIC cxx_std_20)

add_library(alya::audio ALIAS alya_audio)

