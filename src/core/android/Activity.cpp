// -*- C++ -*-
#include <core/android/Activity.h>
#include <core/Log.h>

#define LOG_TAG "Activity"

const int DEFAULT_FPS = 60;

Activity::Activity(android_app *androidApp, App* app)
        : androidApp(androidApp), app(app)
{
    LOGD("Activity", "ACTIVITY::CONSTRUCTOR");

    // EGL Window - the surface we draw to
    window = unique_ptr<EGLWindow>(new EGLWindow(androidApp));
    window->setFramerateLimit(DEFAULT_FPS);
}
Activity::~Activity()
{
    if (app)
    {
        delete app;
    }
}

int32_t Activity::handleInput(AInputEvent *event)
{
    return 0;
}

static int32_t handle_input(struct android_app *androidApp, AInputEvent *event)
{
    return ((Activity *) androidApp->userData)->handleInput(event);
}

void Activity::handleCmd(int32_t cmd)
{
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
            // TODO: save state support
            break;

        case APP_CMD_CONFIG_CHANGED:
            LOGD("Activity", "APP_CMD_CONFIG_CHANGED");
            break;

        case APP_CMD_INIT_WINDOW:
            LOGD("Activity", "APP_CMD_INIT_WINDOW");
            if (androidApp->window != nullptr)
            {
                window->init();
                app->init(window->getSize());
            }
            break;

        case APP_CMD_TERM_WINDOW:
            LOGD("Activity", "APP_CMD_TERM_WINDOW");
            window->destroy();
            break;

        case APP_CMD_START:
            LOGD("Activity", "APP_CMD_START");
            break;

        case APP_CMD_RESUME:
            LOGD("Activity", "APP_CMD_RESUME");
            break;

        case APP_CMD_PAUSE:
            LOGD("Activity", "APP_CMD_PAUSE");
            break;

        case APP_CMD_STOP:
            LOGD("Activity", "APP_CMD_STOP");
            break;

        case APP_CMD_DESTROY:
            LOGD("Activity", "APP_CMD_DESTROY");
            break;
    }
}

static void handle_cmd(struct android_app *androidApp, int32_t cmd)
{
    ((Activity *) androidApp->userData)->handleCmd(cmd);
}

void Activity::run()
{
    androidApp->userData = this;
    androidApp->onAppCmd = handle_cmd;
    androidApp->onInputEvent = handle_input;

    // TODO: restore state

    // Main loop
    while (1)
    {
        int ident;
        int events;
        android_poll_source *source;

        // Process events
        while ((ident = ALooper_pollAll(0, nullptr, &events, (void **) &source)) >= 0)
        {
            if (source != nullptr)
            {
                source->process(androidApp, source);
            }

            //We are exiting
            if (androidApp->destroyRequested != 0)
            {
                window->destroy();
                return;
            }
        }


        if (window->hasDisplay())
        {
            float frameTime = window->getFrameTime();

            app->render();

            window->swapBuffers();
        }
    }
}
