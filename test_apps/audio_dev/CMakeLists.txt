project(audio_dev)

add_executable(audio_dev main.cpp)

target_compile_features(audio_dev PRIVATE cxx_std_20)

target_link_libraries(audio_dev PUBLIC alya::audio alya::resource)
