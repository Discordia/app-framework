// -*- C++ -*-
#include <core/android/EGLWindow.h>
#include <core/Log.h>
#include <core/OpenGL.h>

#include <thread>

#include <android_native_app_glue.h>
#include <unistd.h>

static const Logger LOGGER = Logger::create("EGLWindow");

EGLWindow::EGLWindow(android_app *app)
        : app(app),
          display(EGL_NO_DISPLAY),
          surface(EGL_NO_SURFACE),
          context(EGL_NO_CONTEXT),
          frameLimit(0),
          frameTime(0)
{
}

void EGLWindow::init()
{
    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] =
    {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
    };

    EGLint dummy, format;
    EGLint numConfigs;
    EGLConfig config;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, app->window, nullptr);

    int attrib_list[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, attrib_list);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
    {
        LOGGER.logf(LOG_ERROR, "Unable to eglMakeCurrent");

        // TODO: proper error handling
        return;
    }

    // Query width and height of surface
    int width, height;
    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);
    this->windowSize.width = width;
    this->windowSize.height = height;

    eglSwapInterval(display, 1);

    LOGGER.logf(LOG_INFO, "Frame Buffer size [%i,%i]", width, height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DITHER);

    glViewport(0, 0, width, height);

    LOGGER.logf(LOG_INFO, "Initalized");
}

void EGLWindow::destroy()
{
    LOGGER.logf(LOG_DEBUG, "destroy()");

    if (display != EGL_NO_DISPLAY)
    {
        LOGGER.logf(LOG_DEBUG, "making no dsiaply current");

        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (context != EGL_NO_CONTEXT)
        {
            LOGGER.logf(LOG_DEBUG, "destroying context");

            eglDestroyContext(display, context);
        }

        if (surface != EGL_NO_SURFACE)
        {
            LOGGER.logf(LOG_DEBUG, "destroying surface");

            eglDestroySurface(display, surface);
        }

        LOGGER.logf(LOG_DEBUG, "terminating display");

        eglTerminate(display);
    }

    display = EGL_NO_DISPLAY;
    surface = EGL_NO_SURFACE;
    context = EGL_NO_CONTEXT;
}

bool EGLWindow::hasDisplay() const
{
    return display != EGL_NO_DISPLAY;
}

void EGLWindow::swapBuffers()
{
    // Limit the framerate if needed
    if (frameLimit > 0)
    {
        milliseconds defaultFrameTime = milliseconds(1000 / frameLimit);
        milliseconds elapsedTime = timer.elapsedTimeMs();

        if (elapsedTime < defaultFrameTime)
        {
            milliseconds remainingFrameTime = defaultFrameTime - elapsedTime;
            std::this_thread::sleep_for(remainingFrameTime);
        }
    }

    frameTime = timer.elapsedTimeMs();
    timer.reset();

    eglSwapBuffers(display, surface);
}

float EGLWindow::getFrameTime() const
{
    return static_cast<float>(frameTime.count());
}

void EGLWindow::setFramerateLimit(const int32_t fpsLimit)
{
    this->frameLimit = fpsLimit;
}

const Dimension& EGLWindow::getSize() const
{
    return windowSize;
}
