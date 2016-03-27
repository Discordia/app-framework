#include <core/android/Activity.h>
#include <core/Log.h>

static const Logger LOGGER = Logger::create("Activity");

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
            LOGGER.logf(LOG_DEBUG, "APP_CMD_SAVE_STATE");
            break;

        case APP_CMD_CONFIG_CHANGED:
            LOGGER.logf(LOG_DEBUG, "APP_CMD_CONFIG_CHANGED");
            break;

        case APP_CMD_INIT_WINDOW:
            LOGGER.logf(LOG_DEBUG, "APP_CMD_INIT_WINDOW");
            if (androidApp->window != nullptr)
            {
                window->init();
                app->init(window->getSize());
            }
            break;

        case APP_CMD_TERM_WINDOW:
            LOGGER.logf(LOG_DEBUG, "APP_CMD_TERM_WINDOW");
            window->destroy();
            app->destroy();
            break;

        case APP_CMD_START:
            LOGGER.logf(LOG_DEBUG, "APP_CMD_START");
            break;

        case APP_CMD_RESUME:
            LOGGER.logf(LOG_DEBUG, "APP_CMD_RESUME");
            resumed = true;
            app->resume();
            break;

        case APP_CMD_PAUSE:
            LOGGER.logf(LOG_DEBUG, "APP_CMD_PAUSE");
            resumed = false;
            app->pause();
            break;

        case APP_CMD_STOP:
            LOGGER.logf(LOG_DEBUG, "APP_CMD_STOP");
            break;

        case APP_CMD_DESTROY:
            LOGGER.logf(LOG_DEBUG, "APP_CMD_DESTROY");
            break;

        case APP_CMD_GAINED_FOCUS:
            LOGGER.logf(LOG_DEBUG, "APP_CMD_GAINED_FOCUS");
            hasFocus = true;
            break;

        case APP_CMD_LOST_FOCUS:
            LOGGER.logf(LOG_DEBUG, "APP_CMD_LOST_FOCUS");
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
                LOGGER.logf(LOG_DEBUG, "Destroy requested, exiting...");
                app->destroy();
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
