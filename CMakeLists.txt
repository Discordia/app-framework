cmake_minimum_required(VERSION 2.8)
project(app_framework)

if(ANDROID)
    include("$ENV{ANDROID_CMAKE_HOME}/android.apk.cmake" REQUIRED)
endif()

# compiler flags
set(CMAKE_CXX_FLAGS
        "-std=c++14 -ffor-scope -fno-exceptions -pipe -ffunction-sections -fdata-sections -ffast-math -pedantic -Wall -Werror -Wnon-virtual-dtor -Wsign-promo -fvisibility=hidden -fvisibility-inlines-hidden -Os -ftree-vectorize")

#
# Dependencies
#
add_subdirectory(src/core)

#includes
include_directories(src)

# for android include native_app_glue
if(ANDROID)
    include_directories($ENV{ANDROID_NDK_HOME}/sources/android/native_app_glue)
endif()

# source
set(SOURCE_FILES
        src/Game.cpp
        src/Game.h)

if(ANDROID)
    add_library(app_framework SHARED ${SOURCE_FILES})
    target_link_libraries(app_framework app_framework_library log android EGL GLESv2 z)
    set_target_properties(app_framework PROPERTIES COMPILE_DEFINITIONS "ANDROID")

    android_create_apk(app_framework "org.discordia.prototype.appframework" "${CMAKE_BINARY_DIR}/apk" "${CMAKE_SOURCE_DIR}/libs" "${CMAKE_SOURCE_DIR}/android" "${CMAKE_SOURCE_DIR}/assets")
elseif(APPLE)
    # GLFW
    find_package(glfw3 REQUIRED)
    include_directories(${GLFW_INCLUDE_DIRS})

    find_package(GLEW REQUIRED)
    include_directories(${GLEW_INCLUDE_DIRS})

    # app_framework executable
    add_executable(app_framework ${SOURCE_FILES})
    target_link_libraries(app_framework app_framework_library ${GLEW_LIBRARY} glfw3)
    target_link_libraries(app_framework "-framework OpenGL")
else()
    # GLFW
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})

    # GLEW
    find_path(GLEW_INCLUDE_DIR GL/glew.h)
    find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib64)

    # app_framework executable
    add_executable(app_framework ${SOURCE_FILES})
    target_link_libraries(app_framework app_framework_library ${GLEW_LIBRARY} ${GLFW_STATIC_LIBRARIES})

    # Copy assets
    add_custom_command(TARGET app_framework PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/assets")
    add_custom_command(TARGET app_framework PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/")
    add_custom_command(TARGET app_framework POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets/")
endif()

