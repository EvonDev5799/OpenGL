#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>

using namespace glimac;

struct Vertex2DUV
{
    glm::vec2 position;
    glm::vec2 uv;

    Vertex2DUV(glm::vec2 position, glm::vec2 uv) : position(position), uv(uv) {}
};

glm::mat3 translate(float tx, float ty)
{
  return glm::mat3(glm::vec3( 1,0,0),glm::vec3(0,1,0),glm::vec3(tx,ty,1));
}

glm::mat3 scale(float sx, float sy)
{
  return glm::mat3(glm::vec3(sx,0,0),glm::vec3(0,sy,0),glm::vec3(0,0,1));
}

glm::mat3 rotate(float a)
{
  float angle = glm::radians(a);
  return glm::mat3(glm::vec3(cos(angle),sin(angle),0),glm::vec3(-sin(angle),cos(angle),0),glm::vec3(0,0,1));
}

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/tex2D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/tex2D.fs.glsl");
    program.use();

    std::unique_ptr<Image> pImage = loadImage("/home/evondev/Desktop/OpenGL/assets/textures/triforce.png");
    if (pImage == NULL)
    {
        std::cerr << "image not fount at " << "/home/evondev/Desktop/OpenGL/assets/textures/triforce.png" << std::endl;
        return EXIT_FAILURE;
    }

    GLint uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    GLint uTexture = glGetUniformLocation(program.getGLId(), "uTexture");

    std::vector<Vertex2DUV> vertices = {
        Vertex2DUV(glm::vec2(-0.25f, -0.25f), glm::vec2(0.f,1.f)),
        Vertex2DUV(glm::vec2( 0.25f, -0.25f), glm::vec2(1.f,1.f)),
        Vertex2DUV(glm::vec2( 0.f,    0.25f), glm::vec2(0.5f, 0.f))

        //Vertex2DUV(glm::vec2( 1.f, -1.f), glm::vec2(0,0)),
        //Vertex2DUV(glm::vec2(-1.f,  1.f), glm::vec2(0,0)),
        //Vertex2DUV(glm::vec2( 1.f,  1.f), glm::vec2(0,0))
    };
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage->getWidth(), pImage->getHeight(), 0, GL_RGBA, GL_FLOAT, pImage->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2DUV) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_UV = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_UV);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), offsetof(Vertex2DUV, position));
    glVertexAttribPointer(VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (void *) offsetof(Vertex2DUV, uv));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUniform1i(uTexture, texture);

    // Application loop:
    bool done = false;
    float angle = 0;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(uTexture, 0);

        glUniformMatrix3fv(uModelMatrix, 1, false, glm::value_ptr(rotate(angle) * translate(-0.5, -0.5)*rotate(angle)));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glUniformMatrix3fv(uModelMatrix, 1, false, glm::value_ptr(rotate(angle) * translate( 0.5, -0.5)*rotate(-2*angle)));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glUniformMatrix3fv(uModelMatrix, 1, false, glm::value_ptr(rotate(angle) * translate(-0.5,  0.5)*rotate(-2 * angle)));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glUniformMatrix3fv(uModelMatrix, 1, false, glm::value_ptr(rotate(angle) * translate( 0.5,  0.5)*rotate(angle)));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        angle += 0.1f;

        // Update the display
        windowManager.swapBuffers();
    }

    glDeleteTextures(1, &texture);

    return EXIT_SUCCESS;
}
