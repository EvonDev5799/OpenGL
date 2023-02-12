#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/common.hpp>
#include <glimac/Cuboid.hpp>
#include <glimac/Program.hpp>
#include <glimac/FreeflyCamera.hpp>
#include <glimac/Track.hpp>
#include <glimac/Rails.hpp>
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
    MonoTexProgram monoTexProgram(applicationPath);
    BiTexProgram biTexProgram(applicationPath);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);

    GLuint cartTexture = makeTexture("../assets/textures/SteelTexture.jpg");
    GLuint railTexture0 = makeTexture("../assets/textures/SteelTexture.jpg");
    GLuint railTexture1 = makeTexture("../assets/textures/RustTexture.jpg");
    
    Cuboid cuboid(glm::vec3(1.f, 1.f, 1.f));
    GLuint cuboidVAO = makeVAO(cuboid.getVertexCount(), cuboid.getDataPointer());    

    std::vector<glm::vec3> points{glm::vec3(0.f, 0.f, 0.f), glm::vec3(2.f, 1.f, 1.f),glm::vec3(2.f, 0.5f, 0.f),glm::vec3(3.f, 0.f, 0.f), glm::vec3(1.5f, 0.f, -1.f)};
    std::vector<glm::vec3> trackPoints = smoothPath(points, 0.2, 10);
    std::vector<glm::vec3> rightPoints = rightTrack(trackPoints, .05f);
    std::vector<glm::vec3> leftPoints = leftTrack(trackPoints, .05f);
    rightPoints = smoothPath(rightPoints, 0.2, 10);
    leftPoints = smoothPath(leftPoints, 0.2, 10);

    Track t(trackPoints);

    Rail railRight(rightPoints, .01f);
    Rail railLeft(leftPoints, .01f);

    GLuint rightVAO = makeVAO(railRight.getVertexCount(), railRight.getDataPointer()); 
    GLuint leftVAO = makeVAO(railLeft.getVertexCount(), railLeft.getDataPointer());

    glm::mat4 projMatrix;
    projMatrix = glm::perspective(glm::radians(70.f), (float)window_params.height / window_params.width, 0.1f, 100.f);
    FreeflyCamera camera;
    camera.moveFront(-5.f);

    glm::ivec2 mousePos = windowManager.getMousePosition();
    float time = windowManager.getTime();

    float friction = 0.9f;
    float speed = 0.f;
    float engineForce = 1.f;
    glm::vec3 g(0, -2.f, 0);

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

        glm::vec4 projected_light_dir = camera.getViewMatrix()*glm::vec4(1.f,1.f,1.f, 0.f);

        glBindVertexArray(cuboidVAO);
        monoTexProgram.m_Program.use();
        glUniform1i(monoTexProgram.uTexture, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cartTexture);

        glm::mat4 rotateMAt = t.currentTransform();
        glm::mat4 MVMatrix = glm::mat4(1.f);
        MVMatrix = glm::translate(MVMatrix, glm::vec3(0.f, .05f, 0.f));
        MVMatrix = MVMatrix * rotateMAt;
        MVMatrix = glm::scale(MVMatrix, glm::vec3(.2f, .1f, .1f));
        MVMatrix = camera.getViewMatrix() * MVMatrix;
        SetUniforms(MVMatrix, projMatrix, monoTexProgram.uMVMatrix, monoTexProgram.uNormalMatrix, monoTexProgram.uMVPMatrix);
        SetLightUniforms(projected_light_dir, monoTexProgram.uLightDir_vs,monoTexProgram.uLightIntensity,monoTexProgram.uKd,monoTexProgram.uKs, monoTexProgram.uShininess);

        glDrawArrays(GL_TRIANGLES, 0, cuboid.getVertexCount());

        MVMatrix = glm::mat4(1.f);
        MVMatrix = glm::translate(MVMatrix, glm::vec3(1.f, -.1f, 1.f));
        MVMatrix = glm::scale(MVMatrix, glm::vec3(2.f, .2f, 2.f));
        MVMatrix = camera.getViewMatrix() * MVMatrix;
        SetUniforms(MVMatrix, projMatrix, monoTexProgram.uMVMatrix, monoTexProgram.uNormalMatrix, monoTexProgram.uMVPMatrix);
        SetLightUniforms(projected_light_dir, monoTexProgram.uLightDir_vs,monoTexProgram.uLightIntensity,monoTexProgram.uKd,monoTexProgram.uKs, monoTexProgram.uShininess);

        glDrawArrays(GL_TRIANGLES, 0, cuboid.getVertexCount());

        glBindVertexArray(rightVAO);
        biTexProgram.m_Program.use();
        glUniform1i(biTexProgram.uTexture0, 0);
        glUniform1i(biTexProgram.uTexture1, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, railTexture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, railTexture1);

        MVMatrix = glm::mat4(1.f);
        MVMatrix = camera.getViewMatrix() * MVMatrix;
        SetUniforms(MVMatrix, projMatrix, biTexProgram.uMVMatrix, biTexProgram.uNormalMatrix, biTexProgram.uMVPMatrix);
        SetLightUniforms(projected_light_dir, biTexProgram.uLightDir_vs,biTexProgram.uLightIntensity,biTexProgram.uKd,biTexProgram.uKs, biTexProgram.uShininess);

        glDrawArrays(GL_TRIANGLES, 0, railRight.getVertexCount());

        glBindVertexArray(leftVAO);

        MVMatrix = glm::mat4(1.f);
        MVMatrix = camera.getViewMatrix() * MVMatrix;
        SetUniforms(MVMatrix, projMatrix, biTexProgram.uMVMatrix, biTexProgram.uNormalMatrix, biTexProgram.uMVPMatrix);

        glDrawArrays(GL_TRIANGLES, 0, railLeft.getVertexCount());     

        glm::ivec2 nextMousePos = windowManager.getMousePosition();
        glm::ivec2 delta = nextMousePos - mousePos;
        mousePos = nextMousePos;

        float nextTime = windowManager.getTime();
        float deltaTime = nextTime - time;
        time = nextTime;

        float acceleration = -friction*speed + glm::dot(t.currentDirection(), g) + engineForce;
        speed += acceleration * deltaTime;

        t.advance(deltaTime * speed);

        if (windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT))
        {
            camera.rotateLeft(50.f * -delta.x * deltaTime);
            camera.rotateUp(50.f * -delta.y * deltaTime);
        }

        if (windowManager.isKeyPressed(SDLK_z))
            camera.moveFront(2.5f*deltaTime);
        if (windowManager.isKeyPressed(SDLK_s))
            camera.moveFront(-2.5f * deltaTime);
        if (windowManager.isKeyPressed(SDLK_d))
            camera.moveLeft(-2.5f * deltaTime);
        if (windowManager.isKeyPressed(SDLK_q))
            camera.moveLeft(2.5f * deltaTime);

        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
