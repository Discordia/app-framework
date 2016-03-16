#pragma once

#include <core/App.h>
#include <core/Dimension.h>
#include <core/StreamFactory.h>

class BufferObject;
class ShaderProgram;

class Game : public App
{
public:
    Game(shared_ptr<StreamFactory> streamFactory);
    ~Game();

    void init(const Dimension& windowSize);
    void render();

private:
    shared_ptr<BufferObject> vertexBuffer;
    shared_ptr<BufferObject> indexBuffer;
    shared_ptr<ShaderProgram> shader;
};
