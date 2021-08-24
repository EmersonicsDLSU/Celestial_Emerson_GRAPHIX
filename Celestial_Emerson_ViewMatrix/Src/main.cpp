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
	window = glfwCreateWindow(1024, 768, "Emerson Celestial", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to load window! \n");
		return -1;
	}

	//instantiate
	InputHandler::initialize();
	//keyboard input
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 2);
	glfwSetKeyCallback(window, InputHandler::getInstance()->keyCallback);
	//mouse input
	glfwSetCursorPosCallback(window, InputHandler::getInstance()->cursorPositionCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
	LoadObjFile(&tourusObjData, "cube.obj");
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


	// initialize MVP
	GLuint modelTransformLoc = glGetUniformLocation(shaderProgram, "u_model"); //model space
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "u_view"); //view space
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "u_projection"); //projection space

	glm::mat4 trans = glm::mat4(1.0f); // identity
	glm::mat4 view = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

	// set bg color to green
	glClearColor(0.4f, 0.4f, 0.0f, 0.0f);

	// var for rotations
	float xFactor = 0.0f;
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;

	while (!glfwWindowShouldClose(window)) {

#pragma region Viewport
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
#pragma endregion

#pragma region Projection
		// Orthopgraphic projection but make units same as pixels. origin is lower left of window
		// projection = glm::ortho(0.0f, (GLfloat)width, 0.0f, (GLfloat)height, 0.1f, 10.0f); // when using this scale objects really high at pixel unity size

		// Orthographic with stretching
		//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 10.0f);

		// Orthographic with corection for stretching, resize window to see difference with previous example
		//projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 0.1f, 10.0f);

		// Perspective Projection
			projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f),

			// Set projection matrix in shader
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

#pragma region View

		// get deltaTime
		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		InputHandler::checkMouse(deltaTime);

		float cameraSpeed = 2.5f * deltaTime;

		if (InputHandler::getInstance()->W_KEY_ON)
		{
			InputHandler::getInstance()->cameraPos += cameraSpeed * InputHandler::getInstance()->cameraFront;
		}
		if (InputHandler::getInstance()->S_KEY_ON)
		{
			InputHandler::getInstance()->cameraPos -= cameraSpeed * InputHandler::getInstance()->cameraFront;
		}
		if (InputHandler::getInstance()->A_KEY_ON)
		{
			InputHandler::getInstance()->cameraPos -= glm::normalize(glm::cross(InputHandler::getInstance()->cameraFront, InputHandler::getInstance()->cameraUp)) * cameraSpeed;
		}
		if (InputHandler::getInstance()->D_KEY_ON)
		{
			InputHandler::getInstance()->cameraPos += glm::normalize(glm::cross(InputHandler::getInstance()->cameraFront, InputHandler::getInstance()->cameraUp)) * cameraSpeed;
		}
		if (InputHandler::getInstance()->Q_KEY_ON)
		{
			InputHandler::getInstance()->cameraPos -= glm::cross(
				glm::normalize(glm::cross(InputHandler::getInstance()->cameraFront, InputHandler::getInstance()->cameraUp)),
				InputHandler::getInstance()->cameraFront) * cameraSpeed;
		}
		if (InputHandler::getInstance()->E_KEY_ON)
		{
			InputHandler::getInstance()->cameraPos += glm::cross(
				glm::normalize(glm::cross(InputHandler::getInstance()->cameraFront, InputHandler::getInstance()->cameraUp)),
				InputHandler::getInstance()->cameraFront) * cameraSpeed;
		}
		//resets inputHandler
		if (InputHandler::getInstance()->KEY_RELEASE)
			InputHandler::getInstance()->reset();
		prevTime = currentTime;

		view = glm::mat4(1.0f); // identity
		//glm::mat3 view = glm::mat3(glm::rotate(-centerX * rot_speed, glm::vec3(0.0f, 1.0f, 0.0f))) * view;

		view = glm::lookAt(
			glm::vec3(InputHandler::getInstance()->cameraPos), //eye
			glm::vec3(InputHandler::getInstance()->cameraPos + InputHandler::getInstance()->cameraFront), //center
			glm::vec3(InputHandler::getInstance()->cameraUp) //up
		);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
#pragma endregion

		glClear(GL_COLOR_BUFFER_BIT);

		//toggle to render wit GL_FILL or GL_LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#pragma region Draw

		//draw bunny
		glBindVertexArray(bunnyObjData.vaoId);

		// transforms
		trans = glm::mat4(1.0f); // identity
		//trans = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans = glm::translate(trans, glm::vec3(xFactor, 0.0f, -3.0f)); // matrix * translate_matrix
		trans = glm::scale(trans, glm::vec3(2.0f, 2.0f, 2.0f));
		//send to shader
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));


		glDrawElements(GL_TRIANGLES, bunnyObjData.numFaces, GL_UNSIGNED_INT, (void*)0);

		//draw tourus
		glBindVertexArray(tourusObjData.vaoId);

		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glDrawElements(GL_TRIANGLES, tourusObjData.numFaces, GL_UNSIGNED_INT, (void*)0);


		//--- stop drawing here ---

		glfwSwapBuffers(window);
#pragma endregion
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}
