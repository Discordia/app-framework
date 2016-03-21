#include <core/android/Activity.h>
#include <core/Log.h>

#define LOG_TAG "Activity"

const int DEFAULT_FPS = 60;

Activity::Activity(android_app *androidApp, App* app)
        : androidApp(androidApp), app(app), resumed(false), hasFocus(false)
{
    window = unique_ptr<EGLWindow>(new EGLWindow(androidApp));
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
            LOGD("Activity", "APP_CMD_SAVE_STATE");
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
            resumed = true;
            break;

        case APP_CMD_PAUSE:
            LOGD("Activity", "APP_CMD_PAUSE");
            resumed = false;
            break;

        case APP_CMD_STOP:
            LOGD("Activity", "APP_CMD_STOP");
            break;

        case APP_CMD_DESTROY:
            LOGD("Activity", "APP_CMD_DESTROY");
            break;

        case APP_CMD_GAINED_FOCUS:
            LOGD("Activity", "APP_CMD_GAINED_FOCUS");
            hasFocus = true;
            break;

        case APP_CMD_LOST_FOCUS:
            LOGD("Activity", "APP_CMD_LOST_FOCUS");
            hasFocus = false;
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

                ANativeActivity_finish(androidApp->activity);
                return;
            }
        }


        if (ready())
        {
            float frameTime = window->getFrameTime();

            app->render(frameTime);

            window->swapBuffers();
        }
    }
}

bool Activity::ready()
{
    return window->hasDisplay() && resumed && hasFocus;
}
