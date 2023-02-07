#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/common.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glimac/TrackballCamera.hpp>
#include <vector>

using namespace glimac;

struct WindowParameters
{

    uint32_t width;
    uint32_t height;
    const char *title;

    WindowParameters(uint32_t width, uint32_t height, const char *title) : width(width), height(height), title(title) {}
};

struct EarthProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
    }
};

struct MoonProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    MoonProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/tex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
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

    std::unique_ptr<Image> pEarthTexture = loadImage("/home/evon/Desktop/GLImac-Template/assets/textures/EarthMap.jpg");
    if (pEarthTexture == NULL)
    {
        std::cerr << "image not fount at " << "/home/evon/Desktop/GLImac-Template/assets/textures/EarthMap.jpg" << std::endl;
        return EXIT_FAILURE;
    }

    std::unique_ptr<Image> pMoonTexture = loadImage("/home/evon/Desktop/GLImac-Template/assets/textures/MoonMap.jpg");
    if (pMoonTexture == NULL)
    {
        std::cerr << "image not fount at " << "//home/evon/Desktop/GLImac-Template/assets/textures/MoonMap.jpg" << std::endl;
        return EXIT_FAILURE;
    }

    std::unique_ptr<Image> pCloudTexture = loadImage("/home/evon/Desktop/GLImac-Template/assets/textures/CloudMap.jpg");
    if (pCloudTexture == NULL)
    {
        std::cerr << "image not fount at " << "//home/evon/Desktop/GLImac-Template/assets/textures/CloudMap.jpg" << std::endl;
        return EXIT_FAILURE;
    }

    GLuint earthTexture;
    glGenTextures(1, &earthTexture);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pEarthTexture->getWidth(), pEarthTexture->getHeight(), 0, GL_RGBA, GL_FLOAT, pEarthTexture->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint moonTexture;
    glGenTextures(1, &moonTexture);
    glBindTexture(GL_TEXTURE_2D, moonTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pMoonTexture->getWidth(), pMoonTexture->getHeight(), 0, GL_RGBA, GL_FLOAT, pMoonTexture->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint cloudTexture;
    glGenTextures(1, &cloudTexture);
    glBindTexture(GL_TEXTURE_2D, cloudTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pCloudTexture->getWidth(), pCloudTexture->getHeight(), 0, GL_RGBA, GL_FLOAT, pCloudTexture->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

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

    FilePath applicationPath(argv[0]);
    EarthProgram earthProgram(applicationPath);
    MoonProgram moonProgram(applicationPath);

    glEnable(GL_DEPTH_TEST);

    glm::mat4 projMatrix;
    projMatrix = glm::perspective(glm::radians(70.f), (float)window_params.height / window_params.width, 0.1f, 100.f);

    TrackballCamera camera;

    std::vector<glm::vec3> rotations;
    for (int i(0); i < 3; i++)
        rotations.push_back(glm::sphericalRand(1.f));

    glm::ivec2 mousePos = windowManager.getMousePosition();

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

        earthProgram.m_Program.use();

        glUniform1i(earthProgram.uEarthTexture, 0);
        glUniform1i(earthProgram.uCloudTexture, 1);

        glm::mat4 globalMVMatrix = glm::mat4(1.f);

        glm::mat4 earthMVMatrix = globalMVMatrix;//glm::rotate(globalMVMatrix, windowManager.getTime(), glm::vec3(0, 1, 0));
        earthMVMatrix = camera.getViewMatrix() * earthMVMatrix;
        glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(earthMVMatrix));
        glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
        glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * earthMVMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cloudTexture);

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        
        moonProgram.m_Program.use();
        glUniform1i(moonProgram.uTexture, 0);
        for(glm::vec3 r : rotations) {

            glm::mat4 moonMVMatrix = globalMVMatrix;
            moonMVMatrix = glm::rotate(globalMVMatrix, windowManager.getTime(), r);
            moonMVMatrix = glm::translate(moonMVMatrix, 2.f * glm::normalize(glm::vec3(r.z, r.z, -r.x-r.y)));
            moonMVMatrix = glm::rotate(moonMVMatrix, windowManager.getTime(), r);
            moonMVMatrix = glm::scale(moonMVMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
            moonMVMatrix = camera.getViewMatrix() * moonMVMatrix;


            glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(moonMVMatrix));
            glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
            glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * moonMVMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, moonTexture);

            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        }

        glm::ivec2 nextMousePos = windowManager.getMousePosition();
        glm::ivec2 delta = nextMousePos - mousePos;
        mousePos = nextMousePos;
        if (windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT))
        {
            camera.rotateLeft(delta.x);
            camera.rotateUp(delta.y);
        }


        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
