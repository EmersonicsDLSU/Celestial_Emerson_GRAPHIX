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
void cursorEnterCallback(GLFWwindow* window, int entered);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mouseComputations(float& rotXFactor, float& rotYFactor, float& rotZFactor, float& transXFactor, float& transYFactor, float& scaleXFactor, float& scaleYFactor, const float& deltaTime);

static int WINDOW_WIDTH = 1068;
static int WINDOW_HEIGHT = 768;

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
	//instantiate
	InputHandler::initialize();
	//keyboard input
	glfwSetKeyCallback(window, InputHandler::getInstance()->keyCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	//mouse input
	glfwSetCursorPosCallback(window, InputHandler::getInstance()->cursorPositionCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorEnterCallback(window, cursorEnterCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);


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
	float rotXFactor = 0.0f;
	float rotYFactor = 0.0f;
	float rotZFactor = 0.0f;
	float transXFactor = 0.0f;
	float transYFactor = -0.25f;
	float scaleXFactor = 4.0f;
	float scaleYFactor = 4.0f;
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


		//resets inputHandler
		if (InputHandler::getInstance()->KEY_RELEASE)
			InputHandler::getInstance()->reset();

		/*
		if (InputHandler::getInstance()->S_KEY_ON)
			std::cout << "S key is Pressed" << std::endl;
		if (InputHandler::getInstance()->T_KEY_ON)
			std::cout << "T key is Pressed" << std::endl;
		if (InputHandler::getInstance()->R_KEY_ON)
			std::cout << "R key is Pressed" << std::endl;
		*/

#pragma region Draw
		//draw bunny
		glBindVertexArray(bunnyObjData.vaoId);

		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;

		if (InputHandler::getInstance()->compute)
			mouseComputations(rotXFactor, rotYFactor, rotZFactor, transXFactor, transYFactor, scaleXFactor, scaleYFactor, deltaTime);

		if (InputHandler::getInstance()->X_KEY_ON)
		{
			rotXFactor += deltaTime * 360.0f;
			if (rotXFactor > 360.0f) {
				rotXFactor -= 360.0f;
			}
		}
		else if (InputHandler::getInstance()->Y_KEY_ON)
		{
			rotYFactor += deltaTime * 360.0f;
			if (rotYFactor > 360.0f) {
				rotYFactor -= 360.0f;
			}
		}

		prevTime = currentTime;

		// transforms
		trans = glm::mat4(1.0f); // identity
		trans = glm::rotate(trans, glm::radians(rotZFactor), glm::vec3(0.0f, 0.0f, 1.0f)); // matrix * rotation_matrix
		trans = glm::rotate(trans, glm::radians(rotXFactor), glm::vec3(1.0f, 0.0f, 0.0f)); // matrix * rotation_matrix
		trans = glm::rotate(trans, glm::radians(rotYFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans = glm::translate(trans, glm::vec3(transXFactor, transYFactor, 0.0f)); // matrix * translate_matrix
		trans = glm::scale(trans, glm::vec3(scaleXFactor, scaleYFactor, 4.0f));
		//glm::mat4 projection = glm::ortho(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		//send to shader
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, bunnyObjData.numFaces, GL_UNSIGNED_INT, (void*)0);

		// incerement rotation by deltaTime

		/*
		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		//rotation speed

		rotFactor += deltaTime * 360.0f;
		if (rotFactor > 360.0f) {
			rotFactor -= 360.0f;
		}

		prevTime = currentTime;*/


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

//when the window is open or close
void cursorEnterCallback(GLFWwindow* window, int entered)
{
	if (entered)
	{

	}
	else
	{

	}
}

//checks the event of the mouse inputs
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{

	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{

	}
}

void mouseComputations(float& rotXFactor, float& rotYFactor, float& rotZFactor, float& transXFactor, float& transYFactor, float& scaleXFactor, float& scaleYFactor, const float& deltaTime)
{
	if (!InputHandler::getInstance()->onceCompute)
	{
		InputHandler::getInstance()->onceCompute = true;
		if (InputHandler::getInstance()->T_KEY_ON)
		{
			InputHandler::getInstance()->originValX = transXFactor;
			InputHandler::getInstance()->originValY = transYFactor;
		}
		else if (InputHandler::getInstance()->S_KEY_ON)
		{
			InputHandler::getInstance()->originValX = scaleXFactor;
			InputHandler::getInstance()->originValY = scaleYFactor;
		}
		else if (InputHandler::getInstance()->R_KEY_ON)
		{
			InputHandler::getInstance()->originValRot = rotZFactor;
		}
	}

	if (InputHandler::getInstance()->T_KEY_ON)
	{
		transXFactor = InputHandler::getInstance()->originValX + InputHandler::getInstance()->differenceX;
		transYFactor = InputHandler::getInstance()->originValY + InputHandler::getInstance()->differenceY;
	}
	else if (InputHandler::getInstance()->S_KEY_ON)
	{
		scaleXFactor = InputHandler::getInstance()->originValX + InputHandler::getInstance()->differenceX;
		scaleYFactor = InputHandler::getInstance()->originValY + InputHandler::getInstance()->differenceY;
	}
	else if (InputHandler::getInstance()->R_KEY_ON)
	{
		rotZFactor = InputHandler::getInstance()->originValRot + InputHandler::getInstance()->differenceX;
	}
}
