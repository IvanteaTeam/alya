
file(GLOB_RECURSE headers ${ALYA_ROOT}/include/alya/ui/core/*.hpp)

file(GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(alya_ui_core STATIC ${headers} ${src})

target_include_directories(alya_ui_core PUBLIC ${ALYA_ROOT}/include)

target_link_libraries(alya_ui_core PUBLIC alya::async)

target_compile_features(alya_ui_core PUBLIC cxx_std_20)

add_library(alya::ui::core ALIAS alya_ui_core)
