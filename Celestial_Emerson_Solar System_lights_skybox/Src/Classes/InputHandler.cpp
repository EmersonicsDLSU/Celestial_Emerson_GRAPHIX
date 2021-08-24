#include "InputHandler.h"

InputHandler* InputHandler::sharedInstance = NULL;

int InputHandler::WINDOW_WIDTH = 1068;
int InputHandler::WINDOW_HEIGHT = 768;
float InputHandler::differenceX = 0.0f;
float InputHandler::differenceY = 0.0f;
float InputHandler::prevTime = 0.0;
float InputHandler::deltaTime = 0.0f;

void InputHandler::initialize()
{
	sharedInstance = new InputHandler();
}

InputHandler* InputHandler::getInstance()
{
	return sharedInstance;
}

void InputHandler::checkMouse(float deltaTime)
{
	float navSpeed = 30.0f * deltaTime;
	float valX = 0.0f;
	float valY = 0.0f;

	if (InputHandler::getInstance()->diffValX > WINDOW_WIDTH * 0.1f)
	{
		valX = navSpeed;
		std::cout << "Moving Right" << std::endl;
	}
	else if (InputHandler::getInstance()->diffValX < -WINDOW_WIDTH * 0.1f)
	{
		valX = -navSpeed;
		std::cout << "Moving Left" << std::endl;
	}

	if (InputHandler::getInstance()->diffValY > WINDOW_HEIGHT * 0.1f)
	{
		valY = -navSpeed;
		std::cout << "Moving Down" << std::endl;
	}
	else if (InputHandler::getInstance()->diffValY < -WINDOW_HEIGHT * 0.1f)
	{
		valY = navSpeed;
		std::cout << "Moving Up" << std::endl;
	}

	InputHandler::getInstance()->yaw += valX;
	InputHandler::getInstance()->pitch += valY;

	//std::cout << InputHandler::getInstance()->yaw << ":" << InputHandler::getInstance()->pitch << std::endl;

	if (InputHandler::getInstance()->pitch > 89.0f)
		InputHandler::getInstance()->pitch = -89.0f;
	if (InputHandler::getInstance()->pitch < -89.0f)
		InputHandler::getInstance()->pitch = 89.0f;

	glm::vec3 forward = glm::vec3(
		cos(glm::radians(InputHandler::getInstance()->yaw)) * cos(glm::radians(InputHandler::getInstance()->pitch)),
		sin(glm::radians(InputHandler::getInstance()->pitch)),
		sin(glm::radians(InputHandler::getInstance()->yaw)) * cos(glm::radians(InputHandler::getInstance()->pitch))
	);
	InputHandler::getInstance()->cameraFront = glm::normalize(forward);
}

//gets the position of the mouse
void InputHandler::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	InputHandler::getInstance()->diffValX = xPos - WINDOW_WIDTH / 2;
	InputHandler::getInstance()->diffValY = yPos - WINDOW_HEIGHT / 2;
}


/*
* this function will only be called when a key was pressed
5 parameters:
(1)*window � this is the passed window; the instantiated window for rendering
(2)key � this is the ASCII code of the key that was pressed
(3)scanCode -
(4)action � the way of executing the pressed action for the keys
(5)mods -
*/
void InputHandler::keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (key == GLFW_KEY_W && !InputHandler::getInstance()->W_KEY_ON)
		InputHandler::getInstance()->W_KEY_ON = true;
	if (key == GLFW_KEY_A && !InputHandler::getInstance()->A_KEY_ON)
		InputHandler::getInstance()->A_KEY_ON = true;
	if (key == GLFW_KEY_S && !InputHandler::getInstance()->S_KEY_ON)
		InputHandler::getInstance()->S_KEY_ON = true;
	if (key == GLFW_KEY_D && !InputHandler::getInstance()->D_KEY_ON)
		InputHandler::getInstance()->D_KEY_ON = true;
	if (key == GLFW_KEY_Q && !InputHandler::getInstance()->Q_KEY_ON)
		InputHandler::getInstance()->Q_KEY_ON = true;
	if (key == GLFW_KEY_E && !InputHandler::getInstance()->E_KEY_ON)
		InputHandler::getInstance()->E_KEY_ON = true;

	if (action == GLFW_RELEASE)
	{
		InputHandler::getInstance()->KEY_RELEASE = true;
	}
}


void InputHandler::reset()
{
	InputHandler::getInstance()->W_KEY_ON = false;
	InputHandler::getInstance()->A_KEY_ON = false;
	InputHandler::getInstance()->S_KEY_ON = false;
	InputHandler::getInstance()->D_KEY_ON = false;
	InputHandler::getInstance()->Q_KEY_ON = false;
	InputHandler::getInstance()->E_KEY_ON = false;

	//InputHandler::getInstance()->once = false;
	InputHandler::getInstance()->compute = false;
	InputHandler::getInstance()->onceCompute = false;

	InputHandler::getInstance()->KEY_RELEASE = false;
}