#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/common.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Program.hpp>
#include <glimac/FreeflyCamera.hpp>
#include <vector>

using namespace glimac;

int main(int argc, char **argv)
{
    WindowParameters window_params = WindowParameters(800, 600, "GLImac");

    // Initialize SDL and open a window
    SDLWindowManager windowManager(window_params.height, window_params.width, window_params.title);

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if (GLEW_OK != glewInitError)
    {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
        applicationPath.dirPath() + "shaders/directionallight.fs.glsl");
    program.use();

    Sphere sphere = Sphere(1, 100, 100);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ShapeVertex) * sphere.getVertexCount(), sphere.getDataPointer(), GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEX_COORD = 2;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEX_COORD);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void *)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void *)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEX_COORD, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void *)offsetof(ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    

    glEnable(GL_DEPTH_TEST);

    glm::mat4 projMatrix;
    projMatrix = glm::perspective(glm::radians(70.f), (float)window_params.height / window_params.width, 0.1f, 100.f);

    FreeflyCamera camera;
    camera.moveFront(-5.f);

    std::vector<glm::vec3> rotations;
    for (int i(0); i < 3; i++)
        rotations.push_back(glm::sphericalRand(1.f));

    glm::ivec2 mousePos = windowManager.getMousePosition();
    float time = windowManager.getTime();

    GLuint uMVPMatrix;
    GLuint uMVMatrix;
    GLuint uNormalMatrix;

    GLuint uLightDir_vs;
    GLuint uLightIntensity;
    GLuint uKd;
    GLuint uKs;
    GLuint uShininess;

    uMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

    uLightDir_vs = glGetUniformLocation(program.getGLId(), "uLightDir_vs");
    uLightIntensity = glGetUniformLocation(program.getGLId(), "uLightIntensity");
    uKd = glGetUniformLocation(program.getGLId(), "uKd");
    uKs = glGetUniformLocation(program.getGLId(), "uKs");
    uShininess = glGetUniformLocation(program.getGLId(), "uShininess");

    glUniform3f(uLightIntensity, 1.f, 1.f, 1.f);

    // Application loop:
    bool done = false;
    while (!done)
    {
        // Event loop:
        SDL_Event e;
        while (windowManager.pollEvent(e))
        {
            if (e.type == SDL_QUIT)
            {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);

        glm::mat4 globalMVMatrix = glm::mat4(1.f);

        glm::mat4 MVMatrix = globalMVMatrix;
        MVMatrix = camera.getViewMatrix() * MVMatrix;
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(MVMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * MVMatrix));

        glm::vec4 temp1 = camera.getViewMatrix()*glm::vec4(1.f,1.f,1.f, 0.f);
        glUniform3f(uLightDir_vs, temp1.x, temp1.y, temp1.z);
        glUniform3f(uKd, 1.f,1.f,1.f);
        glUniform3f(uKs, 0.f,0.f,0.f);
        glUniform1f(uShininess, 1.f);

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        /*
        for(glm::vec3 r : rotations) {

            glm::mat4 moonMVMatrix = globalMVMatrix;
            moonMVMatrix = glm::rotate(globalMVMatrix, windowManager.getTime(), r);
            moonMVMatrix = glm::translate(moonMVMatrix, 2.f * glm::normalize(glm::vec3(r.z, r.z, -r.x-r.y)));
            moonMVMatrix = glm::rotate(moonMVMatrix, windowManager.getTime(), r);
            moonMVMatrix = glm::scale(moonMVMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
            moonMVMatrix = camera.getViewMatrix() * moonMVMatrix;


            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(moonMVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * moonMVMatrix));

            temp2 = glm::sphericalRand(1.f);
            glUniform3f(uKd, temp2.x, temp2.y, temp2.z);
            temp2 = glm::sphericalRand(1.f);
            glUniform3f(uKs, temp2.x, temp2.y, temp2.z);
            temp2 = glm::sphericalRand(1.f);
            glUniform1f(uShininess, (temp2.z+1)/2);

            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        }*/

        glm::ivec2 nextMousePos = windowManager.getMousePosition();
        glm::ivec2 delta = nextMousePos - mousePos;
        mousePos = nextMousePos;

        float nextTime = windowManager.getTime();
        float deltaTime = nextTime - time;
        time = nextTime;

        if (windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT))
        {
            camera.rotateLeft(10.f * -delta.x * deltaTime);
            camera.rotateUp(10.f * -delta.y * deltaTime);
        }

        if (windowManager.isKeyPressed(SDLK_z))
            camera.moveFront(deltaTime);
        if (windowManager.isKeyPressed(SDLK_s))
            camera.moveFront(-deltaTime);
        if (windowManager.isKeyPressed(SDLK_d))
            camera.moveLeft(-deltaTime);
        if (windowManager.isKeyPressed(SDLK_q))
            camera.moveLeft(deltaTime);

        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
