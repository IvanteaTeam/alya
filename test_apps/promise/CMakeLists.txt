project(promise)

add_executable(promise main.cpp)

target_link_libraries(promise PRIVATE alya::async)

target_compile_features(promise PRIVATE cxx_std_20)
