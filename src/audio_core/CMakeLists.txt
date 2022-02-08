
file(GLOB_RECURSE headers ${ALYA_ROOT}/include/alya/audio/core/*.hpp)

file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(alya_audio_core STATIC ${headers} ${src})

target_include_directories(alya_audio_core PUBLIC ${ALYA_ROOT}/include)

target_link_libraries(alya_audio_core PUBLIC alya::utility)

target_link_libraries(alya_audio_core PRIVATE)

target_compile_features(alya_audio_core PUBLIC cxx_std_20)

add_library(alya::audio::core ALIAS alya_audio_core)
