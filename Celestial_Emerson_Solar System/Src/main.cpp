#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
//#include "main.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "obj_mesh.h";
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
	stbi_set_flip_vertically_on_load(true);
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
	window = glfwCreateWindow(1024, 768, "Celestial, Emerson", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to load window! \n");
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
#pragma endregion


#pragma region Mesh Loading

	ObjData earth;
	//backpack.textures = 
	LoadObjFile(&earth, "earth/Earth.obj");
	GLfloat earthOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&earth,
		1.0f,
		earthOffsets
	);

	ObjData moon;
	//backpack.textures = 
	LoadObjFile(&moon, "earth/Earth.obj");
	GLfloat moonOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&moon,
		1.0f,
		moonOffsets
	);

	ObjData sun;
	//backpack.textures = 
	LoadObjFile(&sun, "earth/Earth.obj");
	GLfloat sunOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&sun,
		1.0f,
		sunOffsets
	);
#pragma endregion

#pragma region Shader Loading

	GLuint shaderProgram = LoadShaders("Shaders/vertex.shader", "Shaders/fragment.shader");
	glUseProgram(shaderProgram);

	GLuint colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);


	// initialize MVP
	GLuint modelTransformLoc = glGetUniformLocation(shaderProgram, "u_model");
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "u_view");
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "u_projection");

	//3 transformation for the 3 models
	glm::mat4 trans = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	glm::mat4 trans1 = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans1));
	glm::mat4 trans2 = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans2));

	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));



#pragma endregion

	// set bg color to green
	glClearColor(0.4f, 0.4f, 0.0f, 0.0f);

	// var for rotations
	float xFactor1 = 0.0f, xFactor2 = 0.0f, xFactor3 = 0.0f;
	float xSpeed1 = 60.0f, xSpeed2 = 20.0f, xSpeed3 = 60.0f;
	// timer for the switching view
	float changeViewTimer = 0.0f;
	float changeView = 3.0f;
	bool isChange = false;
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;

	//depth testing
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS); // set the depth test function

	//face culling
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); // set which face to cull
	//glFrontFace(GL_CCW); // set the front face orientation

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
		if (isChange)
		{
			//change to perspective view
			projection = glm::perspective(glm::radians(90.0f), ratio, 0.1f, 100.0f);
		}
		else
		{
			//change to orthographic view
			projection = glm::ortho(-7.0f, 7.0f, -7.0f, 7.0f, 0.1f, 100.0f);
		}

			// Set projection matrix in shader
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

#pragma region View

		// incerement rotation by deltaTime
		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		changeViewTimer += deltaTime * 1.0f;
		if (changeViewTimer >= changeView)
		{
			changeViewTimer = 0.0f;
			isChange = !isChange;
		}

		glm::mat4 view;
		// side look
		if (isChange)
		{
			view = glm::lookAt(
				glm::vec3(0.5f, 0.0f, -7.0f),
				//glm::vec3(0.5f, 0.0f, -1.0f),
				glm::vec3(trans1[3][0], trans1[3][1], trans1[3][2]),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
		}
		//top down look
		else
		{
			view = glm::lookAt(
				glm::vec3(0.5f, 80.0f, -10.0f),
				//glm::vec3(0.5f, 0.0f, -1.0f),
				glm::vec3(trans2[3][0], trans2[3][1], trans2[3][2]),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
		}
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
#pragma endregion

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//toggle to render wit GL_FILL or GL_LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#pragma region Draw
		//////////////EARTH
		glBindVertexArray(earth.vaoId);

		// transforms
		trans1 = glm::mat4(1.0f); // identity
		//rotates it to the origin point in the world where the sun is also positioned; revolves around the Sun
		trans1 = glm::rotate(trans1, glm::radians(xFactor2), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans1 = glm::translate(trans1, glm::vec3(3.0f, 0.0f, -3.0f)); // matrix * translate_matrix
		trans1 = glm::scale(trans1, glm::vec3(0.25f, 0.25f, 0.25f));
		//send to shader
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans1));

		glActiveTexture(GL_TEXTURE0);
		GLuint earthTexture = earth.textures[earth.materials[1].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, earthTexture);

		// incerement rotation by deltaTime
		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		xFactor2 += deltaTime * xSpeed2;

		//draw earth
		glDrawElements(GL_TRIANGLES, earth.numFaces, GL_UNSIGNED_INT, (void*)0);

		//unbindtexture after rendering
		glBindTexture(GL_TEXTURE_2D, 0);

		////////// MOON
		glBindVertexArray(moon.vaoId);

		// transforms
		trans = glm::mat4(1.0f); // identity
		//rotates it to the origin point of the Earth; revolves around the Earth
		trans = glm::rotate(trans1, glm::radians(xFactor1), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans = glm::translate(trans, glm::vec3(3.6f, 0.0f, -3.0f)); // matrix * translate_matrix
		trans = glm::scale(trans, glm::vec3(0.4f, 0.4f, 0.4f));
		//send to shader
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glActiveTexture(GL_TEXTURE0);
		GLuint moonTexture = moon.textures[moon.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, moonTexture);

		// incerement rotation by deltaTime
		xFactor1 += deltaTime * xSpeed1;

		//draw moon
		glDrawElements(GL_TRIANGLES, moon.numFaces, GL_UNSIGNED_INT, (void*)0);

		//unbindtexture after rendering
		glBindTexture(GL_TEXTURE_2D, 0);

		////////// SUN
		glBindVertexArray(sun.vaoId);

		// transforms
		trans2 = glm::mat4(1.0f); // identity
		//trans2 = glm::translate(trans2, glm::vec3(-0.8f, 0.0f, -3.0f)); // matrix * translate_matrix
		trans2 = glm::scale(trans2, glm::vec3(0.7f, 0.7f, 0.7f));
		//rotates itself; no revolution to other planets
		trans2 = glm::rotate(trans2, glm::radians(xFactor3), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		//send to shader
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans2));

		glActiveTexture(GL_TEXTURE0);
		GLuint sunTexture = sun.textures[sun.materials[2].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, sunTexture);

		// incerement rotation by deltaTime
		xFactor3 += deltaTime * xSpeed3;
		prevTime = currentTime;

		//draw sun
		glDrawElements(GL_TRIANGLES, sun.numFaces, GL_UNSIGNED_INT, (void*)0);

		//unbindtexture after rendering
		glBindTexture(GL_TEXTURE_2D, 0);

		//--- stop drawing here ---
#pragma endregion

		glfwSwapBuffers(window);
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}