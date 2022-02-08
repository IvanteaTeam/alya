

file(GLOB_RECURSE headers ${ALYA_ROOT}/include/alya/utility/*.hpp)

file(GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(alya_utility STATIC ${headers} ${src})

target_include_directories(alya_utility PUBLIC ${ALYA_ROOT}/include)

target_compile_definitions(alya_utility PUBLIC _WIN32_WINNT=0x0601)

find_package(utf8cpp QUIET REQUIRED)
find_package(glm QUIET REQUIRED)

target_link_libraries(alya_utility PUBLIC utf8cpp::utf8cpp glm::glm)

target_compile_features(alya_utility PUBLIC cxx_std_20)

add_library(alya::utility ALIAS alya_utility)


