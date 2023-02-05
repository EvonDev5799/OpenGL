#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/common.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Program.hpp>
#include <vector>

using namespace glimac;

struct WindowParameters {

    uint32_t width;
    uint32_t height;
    const char* title;

    WindowParameters(uint32_t width, uint32_t height, const char* title) : width(width), height(height), title(title) {}
};

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

    Sphere s = Sphere(1, 10, 10);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ShapeVertex) * s.getVertexCount(), s.getDataPointer(), GL_STATIC_DRAW);

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

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/normals.fs.glsl");
    program.use();

    GLint uMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLint uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    glm::mat4 projMatrix, MVMatrix, normalMatrix;
    projMatrix = glm::perspective(glm::radians(70.f), (float) window_params.height / window_params.width, 0.1f, 100.f);

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

        MVMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.f)); // Translation
        normalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));        
        glDrawArrays(GL_TRIANGLES, 0, s.getVertexCount());
        
        MVMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.f)); // Translation
        MVMatrix = glm::rotate(MVMatrix, windowManager.getTime(), glm::vec3(0.f, 1.f, 0.f)); // Translation * Rotation
        MVMatrix = glm::translate(MVMatrix, glm::vec3(-2.f, 0.f, 0.f)); // Translation * Rotation * Translation
        //MVMatrix = glm::scale(MVMatrix, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
        normalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));           
        glDrawArrays(GL_TRIANGLES, 0, s.getVertexCount());

        

        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
