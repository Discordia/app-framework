#include <Game.h>

Game::Game(shared_ptr<StreamFactory> streamFactory)
{

}

Game::~Game()
{

}

void Game::init(const Dimension& windowSize)
{

}

void Game::render()
{

}

App* createApp(shared_ptr<StreamFactory> streamFactory) {
    return new Game(streamFactory);
}




