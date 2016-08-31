cmake_minimum_required(VERSION 2.8)
project(app_framework_library)

# compiler flags
set(CMAKE_CXX_FLAGS
        "-std=c++11 -ffor-scope -fno-exceptions -pipe -ffunction-sections -fdata-sections -ffast-math -pedantic -Werror -Wnon-virtual-dtor -Wsign-promo -fvisibility=hidden -fvisibility-inlines-hidden -Os -ftree-vectorize")

#includes
include_directories(..)

# for android include native_app_glue
if(ANDROID)
    include_directories($ENV{ANDROID_NDK_HOME}/sources/android/native_app_glue)
endif()

# source
set(SOURCE_FILES
        App.h
        BufferObject.cpp
        Dimension.h
        Log.cpp
        OpenGL.cpp
        ShaderObject.cpp
        ShaderProgram.h
        Time.h
        Stream.h
        StreamFactory.h)

if(ANDROID)
    list(APPEND SOURCE_FILES
            android/AndroidMain.cpp
            android/EGLWindow.cpp
            android/Activity.cpp
            android/AndroidStreamFactory.h
            android/AssetStream.cpp
            android/AssetStream.h
            $ENV{ANDROID_NDK_HOME}/sources/android/native_app_glue/android_native_app_glue.c)
else()
    list(APPEND SOURCE_FILES
            desktop/DesktopMain.cpp
            desktop/GLWindow.cpp
            desktop/GLWindow.h
            desktop/Application.cpp
            desktop/Application.h
            desktop/FileStream.h
            desktop/FileStream.cpp
            desktop/DesktopStreamFactory.h)
endif()

add_library(app_framework_library ${SOURCE_FILES})


