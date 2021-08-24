#pragma once
#include <stddef.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class InputHandler
{
public:
	static void initialize();
	static InputHandler* getInstance();

public:
	bool W_KEY_ON = false;
	bool S_KEY_ON = false;
	bool A_KEY_ON = false;
	bool D_KEY_ON = false;
	bool Q_KEY_ON = false;
	bool E_KEY_ON = false;

	bool lookRight = false;
	bool lookLeft = false;
	bool lookUp = false;
	bool lookDown = false;

	bool KEY_ISPRESSED = false;
	bool KEY_ONPRESSED = false;
	bool KEY_RELEASE = true;

public:
	bool once = false;
	bool onceCompute = false;
	bool compute = false;

	//camera
public:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;

	float diffValX = 0.0f;
	float diffValY = 0.0f;
public:
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
	static float differenceX;
	static float differenceY;
	static float prevTime;
	static float deltaTime;

public:
	static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
	static void reset();
	static void checkMouse(float deltaTime);

private:
	static InputHandler* sharedInstance;

};