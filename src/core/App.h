#pragma once

#include <memory>
#include <core/Dimension.h>

using std::shared_ptr;

class StreamFactory;

class App
{
public:
    App() {}

    virtual ~App() {}

    virtual void init(const Dimension& windowSize) {}
    virtual void resume() {}
    virtual void pause() {}
    virtual void destroy() {}

    virtual void render(float frameTime) {}

    virtual void test() {}

    void linkIssueFix();
};

extern App* createApp(shared_ptr<StreamFactory> streamFactory);