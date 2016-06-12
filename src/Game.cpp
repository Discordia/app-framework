#include <Game.h>
#include <core/BufferObject.h>
#include <core/OpenGL.h>
#include <core/ShaderProgram.h>
#include <core/Log.h>

static const Logger LOGGER = Logger::create("Game");

const string vertexShaderSource =
        "attribute vec4 position;\n"
        "uniform mat4 Projection;\n"
        "uniform mat4 ModelView;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = Projection * ModelView * position;\n"
        "}";

const string fragmentShaderSource =
        "uniform vec4 diffuseColor;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "     gl_FragColor = diffuseColor;\n"
        "}";

Game::Game(shared_ptr<StreamFactory> streamFactory)
        : animationX(0), animationStep(10)
{
    appDummy();
}

Game::~Game()
{
}

void Game::init(const Dimension& windowSize)
{
    glClearColor(0.74f, 0.61f, 0.35f, 1.0f);
    glDisable(GL_DEPTH_TEST);

    vertexBuffer = BufferObject::createVertexBuffer(2048);
    indexBuffer = BufferObject::createIndexBuffer(2048);

    //
    // Shader
    //

    shader = ShaderProgram::create();

    shared_ptr<ShaderObject> vertexShader = ShaderObject::create(GL_VERTEX_SHADER, vertexShaderSource);
    if (!vertexShader->isCompiled()) LOGGER.logf(LOG_ERROR, "Vertext shader failed to compile.");
    shader->attachShader(vertexShader);

    shared_ptr<ShaderObject> fragmentShader = ShaderObject::create(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (!fragmentShader->isCompiled()) LOGGER.logf(LOG_ERROR, "Fragment shader failed to compile.");
    shader->attachShader(fragmentShader);

    shader->link();
    if (!shader->isLinked()) LOGGER.logf(LOG_ERROR, "Shader failed to link");

    // Bind vPosition to attribute 0
    glBindAttribLocation(shader->getProgramId(), 0, "position");

    shader->use();

    const GLfloat identity[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
    GLint modelViewLoc = glGetUniformLocation(shader->getProgramId(), "ModelView");
    glUniformMatrix4fv(modelViewLoc, 1, 0, identity);

    GLfloat ortho[16];
    oglOrthof(ortho, 0.0f, windowSize.width, 0.0f, windowSize.height, -1.0f, 1.0f);
    GLint projectionLoc = glGetUniformLocation(shader->getProgramId(), "Projection");
    glUniformMatrix4fv(projectionLoc, 1, 0, ortho);

    float colorV[] = {1.0f, 0.20f, 0.60f, 1.0f};
    GLint diffuseColorLoc = glGetUniformLocation(shader->getProgramId(), "diffuseColor");
    glUniform4fv(diffuseColorLoc, 1, colorV);

    float vertices[] = {
            100.0f, 100.0f,
            100.0f, 200.0f,
            200.0f, 200.0f,
            200.0f, 100.0f
    };

    vertexBuffer->bind();
    vertexBuffer->fill(0, 8 * sizeof(float), vertices);

    GLubyte indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    indexBuffer->bind();
    indexBuffer->fill(0, 6 * sizeof(GLubyte), indices);
}

void Game::pause()
{
}


void Game::resume()
{
}

void Game::destroy()
{
    vertexBuffer.reset();
    indexBuffer.reset();
    shader.reset();
}

void Game::render(float frameTime)
{
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // translate
    float modelView[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            animationX, 0, 0, 1
    };

    if (animationX > 800) animationStep = -10;
    else if (animationX < 0) animationStep = 10;

    animationX += animationStep;

    GLint modelViewLoc = glGetUniformLocation(shader->getProgramId(), "ModelView");
    glUniformMatrix4fv(modelViewLoc, 1, 0, modelView);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);

    // Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}

App* createApp(shared_ptr<StreamFactory> streamFactory)
{
    return new Game(streamFactory);
}




