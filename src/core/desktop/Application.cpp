#include <core/desktop/Application.h>
#include <core/desktop/GLWindow.h>

Application::Application(const string& title, const Dimension windowSize, App* app)
    : app(app)
{
    window = unique_ptr<GLWindow>(new GLWindow(title, windowSize, true));
}

Application::~Application()
{
    if (app)
    {
        delete app;
    }
}

int Application::run()
{
    window->init();
    app->init(window->getSize());

    while (window->isOpen())
    {
        float frameTime = window->getFrameTime();

        window->processEvents();

        app->render(frameTime);

        window->swapBuffers();
    }

    window->destroy();

    return EXIT_SUCCESS;
}
