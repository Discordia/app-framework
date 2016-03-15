#ifndef ANDROID_APP_GAME_H
#define ANDROID_APP_GAME_H


#include <core/StreamFactory.h>
#include <core/Dimension.h>

class Game
{
public:
    Game(shared_ptr<StreamFactory> streamFactory);

    void init(const Dimension& windowSize);
    void render();
};


#endif //ANDROID_APP_GAME_H
