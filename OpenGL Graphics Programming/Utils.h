#pragma once
#include <iostream>
#include <glm.hpp>

namespace Utils
{
	static bool CAMERA_ORTHOGRAPHIC_MODE = true;

	//Game window size
	static float SCREEN_WIDTH = 1280.0f;
	static float SCREEN_HEIGHT = 720.0f;
	static float HSCREEN_WIDTH = SCREEN_WIDTH / 2.0f;
	static float HSCREEN_HEIGHT = SCREEN_HEIGHT / 2.0f;

	//True window size (set in the windowResize function callback)
	static float TRUE_SCREEN_WIDTH = SCREEN_WIDTH;
	static float TRUE_SCREEN_HEIGHT	= SCREEN_HEIGHT;
	static float TRUE_HSCREEN_WIDTH  = HSCREEN_WIDTH;
	static float TRUE_HSCREEN_HEIGHT = HSCREEN_HEIGHT;


	//Remaps a value from one number range to another (used to remap mouse co-ord from world space to screen space)
	static double remap(double _value, double _oldMin, double _oldMax, double _newMin, double _newMax)
	{
		if(_oldMin == _oldMax)
		{
			std::cout << "\nError: Remap input value range is zero!\n";
			return 0;
		}

		if (_newMin == _newMax)
		{
			std::cout << "\nError: Remap output value range is zero!\n";
			return 0;
		}

		//Check for reversed input range
		bool reversedInput = false;
		double oldMin = glm::min(_oldMin, _oldMax);
		double oldMax = glm::max(_oldMin, _oldMax);
		if(oldMin != _oldMin)
		{
			reversedInput = true;
		}

		//Check for reversed output range
		bool reversedOutput = false;
		double newMin = glm::min(_newMin, _newMax);
		double newMax = glm::max(_newMin, _newMax);
		if (newMin != _newMin)
		{
			reversedOutput = true;
		}

		double portion = (_value - oldMin) * (newMax - newMin) / (oldMax - oldMin);
		if(reversedInput)
		{
			portion = (oldMin - _value) * (newMax - newMin) / (oldMax - oldMin);
		}

		return (reversedOutput) ? (newMax - portion) : (portion + newMin);
	}
}