#pragma once
#include <vector>
#include "Utils.h"

namespace Objects
{
	static std::vector<float> verticesBoid =
	{
		///Bottom right quad
		//Position			    //Colour			      //Texture co-ords
		  0.0f,  20.0f, 0.0f,   255.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    //Middle top
		-10.0f, -10.0f, 0.0f,   255.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    //Bottom left
		 10.0f, -10.0f, 0.0f,   255.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    //Bottom right
	};

	static std::vector<unsigned int> indicesBoid =
	{
		//triangle							
		0, 2, 1,
	};

	static std::vector<float> verticesBackground =
	{
		///Bottom right quad
		//Position			     //Colour			        //Texture co-ords
		-Utils::HSCREEN_WIDTH, -Utils::HSCREEN_HEIGHT, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f,    //Bottom left
		-Utils::HSCREEN_WIDTH,  Utils::HSCREEN_HEIGHT, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,    //Top left
		 Utils::HSCREEN_WIDTH,  Utils::HSCREEN_HEIGHT, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,    //Top right
		 Utils::HSCREEN_WIDTH, -Utils::HSCREEN_HEIGHT, 0.0f,	 1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,    //Bottom right

		 
	};

	static std::vector<unsigned int> indicesBackground =
	{
		//First quad							
		0, 2, 1,
		0, 3, 2,
	};
}
