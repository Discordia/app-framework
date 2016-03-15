
#if defined(__ANDROID__)

//
// Android
//

#include <core/android/Activity.h>
#include <core/android/AndroidStreamFactory.h>

void android_main(android_app* androidApp)
{
    app_dummy();

    App* app = createApp(shared_ptr<StreamFactory>(new AndroidStreamFactory(androidApp->activity->assetManager)));

    unique_ptr<Activity> activity(new Activity(androidApp, app));
    activity->run();
}

#else

//
// Desktop
//

#include <core/desktop/Application.h>
#include <core/desktop/DesktopStreamFactory.h>

int main()
{
    const Dimension windowSize(1024, 768);

    App* app = createApp(shared_ptr<StreamFactory>(new DesktopStreamFactory("assets")));
    
    unique_ptr<Application> application(new Application("Application example", windowSize, app));
    application->run();
}

#endif
