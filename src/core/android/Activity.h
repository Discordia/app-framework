// -*- C++ -*-
#pragma once

#include <jni.h>
#include <errno.h>

#include <android_native_app_glue.h>

#include <memory>

#include <core/android/EGLWindow.h>
#include <core/Dimension.h>
#include <core/App.h>

using std::unique_ptr;

struct android_app;
struct AInputEvent;

class Game;

class Activity
{
public:

    //!
    //!
    //!
    Activity(android_app *androidApp, App* app);

    //!
    //!
    //!
    ~Activity();

    //!
    //!
    //!
    int32_t handleInput(AInputEvent *event);

    //!
    //!
    //!
    void handleCmd(int32_t cmd);

    //!
    //!
    //!
    void run();

private:

    //!
    //! True if the activity is ready to render, false otherwise
    //!
    bool ready();

private:

    //!
    android_app *androidApp;

    //!
    unique_ptr<EGLWindow> window;

    //!
    App* app;

    //! state flags
    bool resumed;
    bool hasFocus;
};