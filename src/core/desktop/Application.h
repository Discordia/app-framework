// -*- C++ -*-
#pragma once

#include <string>
#include <memory>
#include <core/Dimension.h>
#include <Game.h>

using std::string;
using std::unique_ptr;
using std::shared_ptr;

class GLWindow;

class Application
{
public:

    //!
    //!
    //!
    Application(const string& title, const Dimension windowSize, App* app);

    //!
    //!
    //!
    ~Application();


    //!
    //!
    //!
    int run();

private:

    //!
    unique_ptr<GLWindow> window;

    //!
    App* app;
};
