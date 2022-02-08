project(basic)

add_executable(basic main.cpp)

target_link_libraries(basic PRIVATE alya::ui::core)

target_compile_features(basic PRIVATE cxx_std_20)
