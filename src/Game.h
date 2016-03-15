#ifndef ANDROID_APP_GAME_H
#define ANDROID_APP_GAME_H


#include <core/App.h>
#include <core/Dimension.h>
#include <core/StreamFactory.h>

class Game : public App
{
public:
    Game(shared_ptr<StreamFactory> streamFactory);
    ~Game();

    void init(const Dimension& windowSize);
    void render();
};


#endif //ANDROID_APP_GAME_H
