#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <GL/gl.h>

#include "Classes/VertexArray.h"
#include "Classes/VertexBuffer.h"
#include "Classes/ElementBuffer.h"

void LoadShaders(const std::string vertexFilePath, const std::string fragmentFilePath);
void ProcessInput(GLFWwindow* window);

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main()
{
	if (glfwInit())
	{
		std::cout << "GLFW Initialized." << std::endl;
	}
	else
	{
		std::cout << "GLFW Not Initialized." << std::endl;
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL", NULL, NULL);
	if (window == GL_FALSE)
	{
		std::cout << "Failed to Create a Window." << std::endl;
		
		glfwTerminate();
		return EXIT_FAILURE;
	}
	std::cout << "Created a Window." << std::endl;
	glfwMakeContextCurrent(window);

	gladLoadGL();

	{
		// Three Triangles

		GLfloat vertices[] = {
			-0.5f, -0.5f, 0.0f, // 0
			 0.5f, -0.5f, 0.0f, // 1
			 0.0f, -0.5f, 0.0f, // 2
			-0.25f, 0.0f, 0.0f, // 3
			 0.25f, 0.0f, 0.0f, // 4
			 0.0f,  0.5f, 0.0f  // 5
		};

		GLuint indecies[] = {
			0, 2, 3,
			1, 2, 4,
			3, 4, 5
		};

		// Rectangle

		//GLfloat vertices[] = {
		//	-0.5f, -0.5f, 0.0f,
		//	 0.5f, -0.5f, 0.0f,
		//	 0.5f,  0.5f, 0.0f,
		//	-0.5f,  0.5f, 0.0f
		//};

		//GLuint indecies[] = {
		//	0, 1, 3,
		//	1, 2, 3
		//};


		unsigned int NoOfIndecies = sizeof(indecies) / sizeof(GLuint);

		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		VertexArray va;
		VertexBuffer vb(vertices, sizeof(vertices));

		VertexBufferLayout layout;

		layout.Push<float>(3);
		va.AddBuffer(vb, layout);

		ElementBuffer eb(indecies, NoOfIndecies);

		LoadShaders("Shaders/vertex.shader", "Shaders/fragment.shader");


		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		while (!glfwWindowShouldClose(window))
		{
			ProcessInput(window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			//Drawing Triangles
			va.Bind();
			eb.Bind();
			glDrawElements(GL_TRIANGLES, NoOfIndecies, GL_UNSIGNED_INT, nullptr);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return EXIT_SUCCESS;
}

void LoadShaders(const std::string vertexFilePath, const std::string fragmentFilePath)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string VertexShader;
	std::cout << "Loading Vertex Shader" << std::endl;
	std::ifstream vertexFile(vertexFilePath);
	if (vertexFile.is_open())
	{
		std::stringstream sstream;
		sstream << vertexFile.rdbuf();
		VertexShader = sstream.str();
		vertexFile.close();
	}
	else
	{
		std::cout << "Unable to open Vertex Shader" << std::endl;
	}
	const char* vertexShader = VertexShader.c_str();

	std::string FragmentShader;
	std::cout << "Loading Fragment Shader" << std::endl;
	std::ifstream fragmentFile(fragmentFilePath);
	if (fragmentFile.is_open())
	{
		std::stringstream sstream;
		sstream << fragmentFile.rdbuf();
		FragmentShader = sstream.str();
		fragmentFile.close();
	}
	else
	{
		std::cout << "Unable to open Fragment Shader" << std::endl;
	}
	const char* fragmentShader = FragmentShader.c_str();
	
	std::cout << "Compiling Shaders" << std::endl;
	glShaderSource(vertexShaderID, 1, &vertexShader, NULL);
	glCompileShader(vertexShaderID);

	glShaderSource(fragmentShaderID, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShaderID);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderID);
	glAttachShader(shaderProgram, fragmentShaderID);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void ProcessInput(GLFWwindow* window)
{
	static bool wKeyWasPressed = false;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (!wKeyWasPressed)
		{
			GLint polygonMode[2];
			glGetIntegerv(GL_POLYGON_MODE, polygonMode);
			std::cout << polygonMode[0] << std::endl;

			if (polygonMode[0] == GL_FILL)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else if (polygonMode[0] == GL_LINE)
			{
				glPointSize(5.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			wKeyWasPressed = true;
		}
	}
	else
	{
		wKeyWasPressed = false;
	}
}