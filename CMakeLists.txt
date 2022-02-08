cmake_minimum_required(VERSION 3.20)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)


project(alya)


include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_BINARY_DIR})

set(ALYA_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${ALYA_ROOT}/src/utility)
add_subdirectory(${ALYA_ROOT}/src/ui_core)
add_subdirectory(${ALYA_ROOT}/src/async)
add_subdirectory(${ALYA_ROOT}/src/resource)
add_subdirectory(${ALYA_ROOT}/src/audio_core)
add_subdirectory(${ALYA_ROOT}/src/audio)
add_subdirectory(${ALYA_ROOT}/src/graphics)


set_target_properties(alya_utility
alya_ui_core
alya_async
alya_resource
alya_audio_core
alya_audio
alya_graphics
PROPERTIES FOLDER "alya"
)

add_subdirectory(test_apps)

set_target_properties(basic 
audio_dev 
gl_dev 
PROPERTIES FOLDER "alya/test_apps")
