#pragma once
#include "Utils.h"
#include <freeglut.h>

namespace Input
{
	enum InputState
	{
		INPUT_NULL = 0,
		INPUT_UP = 1,
		INPUT_DOWN = 2,
		INPUT_UP_FIRST = 3,
		INPUT_DOWN_FIRST = 4,
	};

	static int g_mousePosX = (int)(Utils::HSCREEN_WIDTH);
	static int g_mousePosY = (int)(Utils::HSCREEN_HEIGHT);
	static int g_mousePosDifX = 0;
	static int g_mousePosDifY = 0;

	//Mouse Input
	static InputState MouseState[3];

	//Keyboard input
	static InputState KeyState[255];
	static InputState SpecialKeyState[255];

	//Declared inputs to be handled that have an uppercase and lowercase version
	static unsigned char inputs[10] = { 'w', 'a', 's', 'd', 'p', 'W', 'A', 'S', 'D', 'P' };

	static void MouseClick(int _button, int _state, int _x, int _y)
	{
		//Move co-ords from (0, 0) at top left, to (0, 0) at middle of screen
		_x -= (int)(Utils::HSCREEN_WIDTH);
		_y -= (int)(Utils::HSCREEN_HEIGHT);

		//Invert y axis
		_y *= -1;

		if (_button >= 3)
			return;
		g_mousePosDifX = _x - g_mousePosX;
		g_mousePosDifY = _y - g_mousePosY;

		if (Utils::CAMERA_ORTHOGRAPHIC_MODE)
		{
			g_mousePosX = _x;
			g_mousePosY = _y;
		}
		else
		{
			glutWarpPointer((int)Utils::HSCREEN_WIDTH, (int)Utils::HSCREEN_HEIGHT);
			g_mousePosX = (int)Utils::HSCREEN_WIDTH;
			g_mousePosY = (int)Utils::HSCREEN_HEIGHT;
		}

		MouseState[_button] = (GLUT_DOWN == _state) ? INPUT_DOWN_FIRST : INPUT_UP_FIRST;
		//Debug logging
		//std::cout << "Mouse clicked on - x: " << _x << " | y: " << _y << std::endl;
	}

	static void MousePassiveMove(int _x, int _y)
	{
		//Convert current screen width and height mouse co-ords to 
		//Move co-ords from (0, 0) at top left, to (0, 0) at middle of screen
		_x = (int)(Utils::remap(_x, -Utils::TRUE_HSCREEN_WIDTH, Utils::TRUE_HSCREEN_WIDTH, -Utils::HSCREEN_WIDTH, Utils::HSCREEN_WIDTH) - Utils::HSCREEN_WIDTH);
		_y = (int)(Utils::remap(_y, -Utils::TRUE_HSCREEN_HEIGHT, Utils::TRUE_HSCREEN_HEIGHT, -Utils::HSCREEN_HEIGHT, Utils::HSCREEN_HEIGHT) - Utils::HSCREEN_HEIGHT);

		//Invert y axis
		_y *= -1;

		g_mousePosDifX = _x - g_mousePosX;
		g_mousePosDifY = _y - g_mousePosY;

		if (Utils::CAMERA_ORTHOGRAPHIC_MODE)
		{
			g_mousePosX = _x;
			g_mousePosY = _y;
		}
		else
		{
			glutWarpPointer((int)Utils::HSCREEN_WIDTH, (int)Utils::HSCREEN_HEIGHT);
			g_mousePosX = (int)Utils::HSCREEN_WIDTH;
			g_mousePosY = (int)Utils::HSCREEN_HEIGHT;
		}
		//Debug logging
		//std::cout << "Mouse moved to - x: " << _x << " | y: " << _y << std::endl;
		//std::cout << "Mouse change in - x: " << g_mousePosDifX << " | y: " << g_mousePosDifY << std::endl;
	}

	static void MouseMove(int _x, int _y)
	{
		//Move co-ords from (0, 0) at top left, to (0, 0) at middle of screen
		_x -= (int)Utils::HSCREEN_WIDTH;
		_y -= (int)Utils::HSCREEN_HEIGHT;

		//Invert y axis
		_y *= -1;

		g_mousePosDifX = _x - g_mousePosX;
		g_mousePosDifY = _y - g_mousePosY;

		if (Utils::CAMERA_ORTHOGRAPHIC_MODE)
		{
			g_mousePosX = _x;
			g_mousePosY = _y;
		}
		else
		{
			glutWarpPointer((int)Utils::HSCREEN_WIDTH, (int)Utils::HSCREEN_HEIGHT);
			g_mousePosX = (int)Utils::HSCREEN_WIDTH;
			g_mousePosY = (int)Utils::HSCREEN_HEIGHT;
		}
		//Debug logging
		//std::cout << "Mouse clicked on - x: " << _x << " | y: " << _y << std::endl;
		//std::cout << "Mouse change in - x: " << g_mousePosDifX << " | y: " << g_mousePosDifY << std::endl;
	}

	static void KeyBoardUp(unsigned char _key, int _x, int _y)
	{
		const InputState tempState = KeyState[_key];

		//Find offset of the corresponding uppercase or lowercase key
		const int offset = (_key < 97) ? 32 : -32;

		//If the key is down
		if (tempState != INPUT_UP && tempState != INPUT_UP_FIRST)
		{
			//Loop through the currently handled input keys
			for (unsigned char input : inputs)
			{
				//If the key is declared to be handled
				if (_key == input)
				{
					//Set the the corresponding uppercase or lowercase key aswell
					KeyState[_key + offset] = INPUT_UP_FIRST;
					break;
				}
			}
			//Continue to change keystate as normal
			KeyState[_key] = INPUT_UP_FIRST;
		}
	}

	static void KeyBoardDown(unsigned char _key, int _x, int _y)
	{
		const InputState tempState = KeyState[_key];
		const int offset = (_key < 97) ? 32 : -32;

		//Key is up
		if (tempState != INPUT_DOWN && tempState != INPUT_DOWN_FIRST)
		{
			for (unsigned char input : inputs)
			{
				if (_key == input)
				{
					KeyState[_key + offset] = INPUT_DOWN_FIRST;
					break;
				}
			}
			KeyState[_key] = INPUT_DOWN_FIRST;
		}
	}

	//Special key up function
	static void SpecialKeyBoardUp(int _key, int _x, int _y)
	{
		const InputState tempState = SpecialKeyState[_key];

		if (tempState != INPUT_UP && tempState != INPUT_UP_FIRST)
		{
			SpecialKeyState[_key] = INPUT_UP_FIRST;
		}
	}

	//Special key down function
	static void SpecialKeyBoardDown(int _key, int _x, int _y)
	{
		const InputState tempState = SpecialKeyState[_key];

		if (tempState != INPUT_DOWN && tempState != INPUT_DOWN_FIRST)
		{
			SpecialKeyState[_key] = INPUT_DOWN_FIRST;
		}
	}

	static void WindowResize(int _x, int _y)
	{
		Utils::TRUE_SCREEN_WIDTH = (float)_x;
		Utils::TRUE_SCREEN_HEIGHT = (float)_y;
		Utils::TRUE_HSCREEN_HEIGHT = Utils::TRUE_SCREEN_HEIGHT / 2;
		Utils::TRUE_HSCREEN_WIDTH = Utils::TRUE_SCREEN_WIDTH / 2;

		glViewport((GLsizei)0, (GLsizei)0, (GLsizei)Utils::TRUE_SCREEN_WIDTH, (GLsizei)Utils::TRUE_SCREEN_HEIGHT);
	}
}
