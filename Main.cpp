#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <GL/gl.h>


void LoadShaders(const std::string vertexFilePath, const std::string fragmentFilePath);
void ProcessInput(GLFWwindow* window);


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

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (window == GL_FALSE)
	{
		std::cout << "Failed to Create a Window." << std::endl;
		
		glfwTerminate();
		return EXIT_FAILURE;
	}
	std::cout << "Created a Window." << std::endl;
	glfwMakeContextCurrent(window);

	gladLoadGL();


	LoadShaders("Shaders/vertex.shader", "Shaders/fragment.shader");


	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	GLuint indecies[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint VertexArrayID, VertexBufferID, ElementBufferID;

	glGenVertexArrays(1, &VertexArrayID);
	glGenBuffers(1, &VertexBufferID);
	glGenBuffers(1, &ElementBufferID);

	glBindVertexArray(VertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float),
		(void*)0
	);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glViewport(0, 0, 800, 600);

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Drawing Triangles
		glBindVertexArray(VertexArrayID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
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