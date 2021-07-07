#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "obj_mesh.h";
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Classes/InputHandler.h"

//function prototype/declaration
void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
void reset();

int main() {

#pragma region Initialization
	//initialize glfw
	if (glfwInit() != GLFW_TRUE) {
		fprintf(stderr, "Failed to initialized! \n");
		return -1;
	}

	// set opengl version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window 
	GLFWwindow* window;
	window = glfwCreateWindow(1024, 768, "<Name Here>", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to load window! \n");
		return -1;
	}
	InputHandler::initialize();
	glfwSetKeyCallback(window, keyCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwMakeContextCurrent(window);

	//initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
#pragma endregion


#pragma region Mesh Loading

	ObjData bunnyObjData;
	LoadObjFile(&bunnyObjData, "bunny.obj");
	GLfloat bunnyOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&bunnyObjData,
		1.0f,
		bunnyOffsets
	);

	ObjData tourusObjData;
	LoadObjFile(&tourusObjData, "two_tourus.obj");
	GLfloat tourusOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&tourusObjData,
		1.0f,
		tourusOffsets
	);
#pragma endregion

#pragma region Shader Loading

	GLuint shaderProgram = LoadShaders("Shaders/vertex.shader", "Shaders/fragment.shader");
	glUseProgram(shaderProgram);

	GLuint colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);


	// initialize transform
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "u_transform");
	glm::mat4 trans = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

#pragma endregion

	// set bg color to green
	glClearColor(0.4f, 0.4f, 0.0f, 0.0f);

	// var for rotations
	float rotFactor = 0.0f;
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT);

		//toggle to render wit GL_FILL or GL_LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if(InputHandler::getInstance()->S_KEY_ON)
			std::cout << "S key is Pressed" << std::endl;
		if (InputHandler::getInstance()->T_KEY_ON)
			std::cout << "T key is Pressed" << std::endl;
		if (InputHandler::getInstance()->R_KEY_ON)
			std::cout << "R key is Pressed" << std::endl;
		//resets inputHandler
		if(InputHandler::getInstance()->KEY_RELEASE)
			reset();

#pragma region Draw
		//draw bunny
		glBindVertexArray(bunnyObjData.vaoId);

		// transforms
		trans = glm::mat4(1.0f); // identity
		trans = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f)); // matrix * translate_matrix
		trans = glm::scale(trans, glm::vec3(4.0f, 4.0f, 4.0f));
		glm::mat4 projection = glm::ortho(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		//send to shader
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, bunnyObjData.numFaces, GL_UNSIGNED_INT, (void*)0);

		// incerement rotation by deltaTime
		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		//rotation speed
		rotFactor += deltaTime * 360.0f;
		if (rotFactor > 360.0f) {
			rotFactor -= 360.0f;
		}
		prevTime = currentTime;


		//draw tourus
		//glBindVertexArray(tourusObjData.vaoId);

		//glDrawElements(GL_TRIANGLES, tourusObjData.numFaces, GL_UNSIGNED_INT, (void*)0);


		//--- stop drawing here ---

		glfwSwapBuffers(window);
#pragma endregion
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}

/*
* this function will only be called when a key was pressed
5 parameters:
(1)*window – this is the passed window; the instantiated window for rendering
(2)key – this is the ASCII code of the key that was pressed
(3)scanCode -
(4)action – the way of executing the pressed action for the keys
(5)mods -
*/
void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (key == GLFW_KEY_S) //its just an identifier for the spacebar ascii code
		switch (action) //action will contain the way of execution of the users to the pressed keys
		{
		case GLFW_PRESS:
			//std::cout << "Key is Pressed" << std::endl;
			break;
		case GLFW_REPEAT:
			if (!InputHandler::getInstance()->KEY_ONPRESSED)
			{
				InputHandler::getInstance()->S_KEY_ON = true;
				InputHandler::getInstance()->KEY_ONPRESSED = true;
			}
			//std::cout << "Key is hold" << std::endl;
			break;
		case GLFW_RELEASE:
			if (InputHandler::getInstance()->S_KEY_ON)
			{
				InputHandler::getInstance()->KEY_RELEASE = true;
				InputHandler::getInstance()->KEY_ONPRESSED = false;
			}
			//std::cout << "Key is Release" << std::endl;
			break;
		}

	if (key == GLFW_KEY_T)
		switch (action)
		{
		case GLFW_PRESS:
			//std::cout << "Key is Pressed" << std::endl;
			break;
		case GLFW_REPEAT:
			if (!InputHandler::getInstance()->KEY_ONPRESSED)
			{
				InputHandler::getInstance()->T_KEY_ON = true;
				InputHandler::getInstance()->KEY_ONPRESSED = true;
			}
			//std::cout << "Key is hold" << std::endl;
			break;
		case GLFW_RELEASE:
			if (InputHandler::getInstance()->T_KEY_ON)
			{
				InputHandler::getInstance()->KEY_RELEASE = true;
				InputHandler::getInstance()->KEY_ONPRESSED = false;
			}
			//std::cout << "Key is Release" << std::endl;
			break;
		}

	if (key == GLFW_KEY_R)
		switch (action) 
		{
		case GLFW_PRESS:
			//std::cout << "Key is Pressed" << std::endl;
			break;
		case GLFW_REPEAT:
			if (!InputHandler::getInstance()->KEY_ONPRESSED)
			{
				InputHandler::getInstance()->R_KEY_ON = true;
				InputHandler::getInstance()->KEY_ONPRESSED = true;
			}
			//std::cout << "Key is hold" << std::endl;
			break;
		case GLFW_RELEASE:
			if (InputHandler::getInstance()->R_KEY_ON)
			{
				InputHandler::getInstance()->KEY_RELEASE = true;
				InputHandler::getInstance()->KEY_ONPRESSED = false;
			}
			//std::cout << "Key is Release" << std::endl;
			break;
		}
}

void reset()
{
	InputHandler::getInstance()->R_KEY_ON = false;
	InputHandler::getInstance()->S_KEY_ON = false;
	InputHandler::getInstance()->T_KEY_ON = false;
	InputHandler::getInstance()->KEY_RELEASE = false;
}