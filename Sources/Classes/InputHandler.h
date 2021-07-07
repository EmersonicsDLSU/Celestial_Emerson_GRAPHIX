#pragma once
#include <stddef.h>

class InputHandler
{
public:
	static void initialize();
	static InputHandler* getInstance();
	bool S_KEY_ON = false;
	bool T_KEY_ON = false;
	bool R_KEY_ON = false;
	bool KEY_RELEASE = false;
	bool KEY_ONPRESSED = false;

private:
	static InputHandler* sharedInstance;

};

