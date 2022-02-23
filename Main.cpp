#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"

#include "Texture.h"

const unsigned int width = 800;
const unsigned int height = 800;

//Our vertices
GLfloat vertices[] = {

	-0.5f,		0.0f,		0.5f,			1.0f,	0.0f,	0.0f,			0.0f, 0.0f,
	-0.5f,		0.0f,		-0.5f,			0.0f,	1.0f,	0.0f,			5.0f, 0.0f,
	0.5f,		0.0f,		-0.5f,			0.0f,	0.0f,	1.0f,			0.0f, 0.0f,
	0.5f,		0.0f,		0.5f,			1.0f,	2.0f,	1.0f,			5.0f, 0.0f,
	0.0f,		0.8f,		0.0f,			1.0f,	2.0f,	1.0f,			2.5f, 5.0f

};

//Our incidces of which vertex to use when
GLuint indices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};



int main() {
	
	//Initilize GLFW
	glfwInit();

	//Tell it the version of OpenGL and the profile
	//only have the modern functions in core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window and check if it went okay
	GLFWwindow* window = glfwCreateWindow(width, height, "OPENGL Test", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//introduce the window to the context
	glfwMakeContextCurrent(window);

	//load glad to help OpenGl
	gladLoadGL();

	// Set viewport of opengl inside the window
	glViewport(0, 0, width, height);


	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.UnBind();
	VBO1.UnBind();
	EBO1.UnBind();

	Texture popcat("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popcat.texUnit(shaderProgram, "tex0", 0);
	

	

	//background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	//put the back buffer to the front and the fron to the back
	glfwSwapBuffers(window);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	float prevTime = glfwGetTime();

	//Our main loop that controls all
	while (!glfwWindowShouldClose(window)) {

		float crntTime = glfwGetTime();
		float deltaTime = crntTime - prevTime;
		prevTime = crntTime;

		//draw to the screen
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Tell open gl which shader we want to use
		shaderProgram.Activate();

		camera.Inputs(window, deltaTime);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		

		popcat.Bind();

		//Bind VAO so OpenGl knows to use it
		VAO1.Bind();

		//Draw Shapes
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		//handle all GLFW events
		glfwPollEvents();

	}

	//end everything properly
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	popcat.Delete();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}