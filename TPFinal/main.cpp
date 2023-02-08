#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/common.hpp>
#include <glimac/Cuboid.hpp>
#include <glimac/Program.hpp>
#include <glimac/FreeflyCamera.hpp>
#include <glimac/Track.hpp>
#include <vector>

using namespace glimac;

std::vector<glm::mat4> rails(std::vector<glm::vec3> track_points, int n)
{
    std::vector<glm::mat4> r(2*n);

    Track t(track_points);
    float step_length = t.lenght()/n;
    glm::mat4 scaling = glm::scale(glm::mat4(1.f), step_length*glm::vec3(1.f, .25f, .25f));

    for(int i(0); i < n; i++)
    {
        glm::mat4 base = t.currentTransform();
        glm::vec3 side = glm::normalize(glm::cross(t.currentDirection(), glm::vec3(0.f,1.f,0.f)));

        glm::mat4 transform_side_1(1.f);
        transform_side_1 = glm::translate(transform_side_1, .05f * side);
        transform_side_1 = transform_side_1 * base * scaling;
        r[2*i] = transform_side_1;

        glm::mat4 transform_side_2(1.f);
        transform_side_2 = glm::translate(transform_side_2, -.05f * side);
        transform_side_2 = transform_side_2 * base * scaling;
        r[2*i + 1] = transform_side_2;

        t.advance(step_length);
    }
    return r;
}

std::vector<glm::vec3> smoothPath(std::vector<glm::vec3> track_points, float smooth_range, int smooth_points)
{
    if(smooth_range > 0.5f || smooth_range < 0.f)
        std::cout << "smooth_range value out of [0 , 0.5]" << std::endl;

    std::vector<glm::vec3> r;

    int source_size = track_points.size();
    for(int i(0); i < source_size; i++ )
    {
        glm::vec3 prev = track_points[i];
        glm::vec3 center = track_points[(i + 1)%source_size];
        glm::vec3 next = track_points[(i + 2)%source_size];

        glm::vec3 toprev = prev - center;
        glm::vec3 tonext = next - center;

        glm::vec3 point1 = center  + smooth_range * toprev;
        glm::vec3 point2 = center;
        glm::vec3 point3 = center  + smooth_range * tonext;

        for(int j(0); j < smooth_points + 2; j++)
        {
            float t = ((float)j)/(smooth_points + 1);
            glm::vec3 intermediate1 = (1-t)*point1 + t*point2;
            glm::vec3 intermediate2 = (1-t)*point2 + t*point3;
            glm::vec3 final = (1-t)*intermediate1 + t*intermediate2;

            r.push_back(final);
        }

    }
    return r;
}

struct WindowParameters
{

    uint32_t width;
    uint32_t height;
    const char *title;

    WindowParameters(uint32_t width, uint32_t height, const char *title) : width(width), height(height), title(title) {}
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

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
        applicationPath.dirPath() + "shaders/normals.fs.glsl");
    program.use();
    
    Cuboid sphere(glm::vec3(1.f, 1.f, 1.f));

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

    glm::ivec2 mousePos = windowManager.getMousePosition();
    float time = windowManager.getTime();

    GLuint uMVPMatrix;
    GLuint uMVMatrix;
    GLuint uNormalMatrix;

    uMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

    std::vector<glm::vec3> trackPoints{glm::vec3(0.f, 0.f, 0.f), glm::vec3(2.f, 1.f, 1.f),glm::vec3(2.f, 0.5f, 0.f),glm::vec3(3.f, 0.f, 0.f), glm::vec3(1.5f, 0.f, -1.f)};
    trackPoints = smoothPath(trackPoints, 0.2f, 100);
    Track t(trackPoints);
    std::vector<glm::mat4> rail_transform = rails(trackPoints, 1000);
    
    glm::vec3 globalUp(0.f, 1.f, 0.f);

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

        glm::mat4 rotateMAt = t.currentTransform();

        glm::mat4 MVMatrix = glm::mat4(1.f);
        MVMatrix = glm::translate(MVMatrix, glm::vec3(0.f, .05f, 0.f));
        MVMatrix = MVMatrix * rotateMAt;
        MVMatrix = glm::scale(MVMatrix, glm::vec3(.2f, .1f, .1f));
        MVMatrix = camera.getViewMatrix() * MVMatrix;
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(MVMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * MVMatrix));

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        for(glm::mat4 transform : rail_transform)
        {
            MVMatrix = camera.getViewMatrix() * transform;
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(MVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * MVMatrix));

            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        }

        /*
        MVMatrix = glm::mat4(1.f);
        MVMatrix = glm::translate(MVMatrix, glm::vec3(2.5f,-.5f,-1.f));
        MVMatrix = glm::scale(MVMatrix, glm::vec3(5.f, 1.f, 2.f));
        MVMatrix = camera.getViewMatrix() * MVMatrix;
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(MVMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * MVMatrix));

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        */

        glm::ivec2 nextMousePos = windowManager.getMousePosition();
        glm::ivec2 delta = nextMousePos - mousePos;
        mousePos = nextMousePos;

        float nextTime = windowManager.getTime();
        float deltaTime = nextTime - time;
        time = nextTime;

        t.advance(-deltaTime);

        if (windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT))
        {
            camera.rotateLeft(50.f * -delta.x * deltaTime);
            camera.rotateUp(50.f * -delta.y * deltaTime);
        }

        if (windowManager.isKeyPressed(SDLK_z))
            camera.moveFront(10.f*deltaTime);
        if (windowManager.isKeyPressed(SDLK_s))
            camera.moveFront(-10.f * deltaTime);
        if (windowManager.isKeyPressed(SDLK_d))
            camera.moveLeft(-10.f * deltaTime);
        if (windowManager.isKeyPressed(SDLK_q))
            camera.moveLeft(10.f * deltaTime);

        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
