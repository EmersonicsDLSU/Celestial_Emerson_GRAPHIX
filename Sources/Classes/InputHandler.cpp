#include "InputHandler.h"

InputHandler* InputHandler::sharedInstance = NULL;

void InputHandler::initialize()
{
	sharedInstance = new InputHandler();
}

InputHandler* InputHandler::getInstance()
{
	return sharedInstance;
}