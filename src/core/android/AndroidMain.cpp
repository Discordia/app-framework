#include <core/android/Activity.h>
#include <core/android/AndroidStreamFactory.h>
#include <core/App.h>

void android_main(android_app* androidApp)
{
    app_dummy();

    App* app = createApp(shared_ptr<StreamFactory>(new AndroidStreamFactory(androidApp->activity->assetManager)));

    unique_ptr<Activity> activity(new Activity(androidApp, app));
    activity->run();
}

void App::linkIssueFix()
{
}

