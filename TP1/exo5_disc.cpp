#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <vector>

using namespace glimac;

struct Vertex2DColor
{
	glm::vec2 position;
	glm::vec3 color;

	Vertex2DColor(){};
	Vertex2DColor(glm::vec2 position, glm::vec3 color) : position(position), color(color){};
};

int main(int argc, char **argv)
{
	// Initialize SDL and open a window
	SDLWindowManager windowManager(800, 600, "GLImac");

	// Initialize glew for OpenGL3+ support
	GLenum glewInitError = glewInit();
	if (GLEW_OK != glewInitError)
	{
		std::cerr << glewGetErrorString(glewInitError) << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

	FilePath applicationPath(argv[0]);
	Program program = loadProgram(applicationPath.dirPath() + "shaders/triangle.vs.glsl",
								  applicationPath.dirPath() + "shaders/triangle.fs.glsl");
	program.use();

	/*********************************
	 * HERE SHOULD COME THE INITIALIZATION CODE
	 *********************************/
	int n = 8;

	std::vector<Vertex2DColor> vertices;

	glm::vec3 outer_color, inner_color;
	outer_color = glm::vec3(1.0f, 0.0f, 0.0f);
	inner_color = glm::vec3(0.0f, 1.0f, 0.0f);

	float angle;
	glm::vec2 center, a, b;

	center = glm::vec2(0,0);
	a = glm::vec2(1.0f/4, 0.0f/3);

	for (int i(1); i <= n; i++)
	{
		angle = (2 * i * glm::pi<float>()) / n;
		b = a;
		a = glm::vec2(glm::cos(angle)/4, glm::sin(angle)/3);


		vertices.push_back(Vertex2DColor(center, inner_color));
		vertices.push_back(Vertex2DColor(b, outer_color));
		vertices.push_back(Vertex2DColor(a, outer_color));
	}


	GLuint vbo, vao;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2DColor) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	const GLuint VERTEX_ATTR_POSITION = 3;
	const GLuint VERTEX_ATTR_COLOR = 8;
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
	glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), offsetof(Vertex2DColor, position));
	glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (void *)offsetof(Vertex2DColor, color));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, n  * 3);
		glBindVertexArray(0);

		// Update the display
		windowManager.swapBuffers();
	}
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
