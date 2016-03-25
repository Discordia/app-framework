#include <core/desktop/Application.h>
#include <core/desktop/DesktopStreamFactory.h>
#include <core/App.h>

int main()
{
    const Dimension windowSize(1024, 768);

    App* app = createApp(shared_ptr<StreamFactory>(new DesktopStreamFactory("assets")));

    unique_ptr<Application> application(new Application("Application example", windowSize, app));
    application->run();
}

void App::appDummy()
{
}