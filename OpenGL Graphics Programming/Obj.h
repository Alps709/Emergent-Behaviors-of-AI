#pragma once
#include <vector>
#include "Utils.h"

namespace Objects
{
	//Scrolling texture
	static std::vector<float> verticesSnekPart =
	{
		///Bottom right quad
		//Position			     //Colour			        //Texture co-ords
		-20.0f, -20.0f, 0.0f,	 1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,    //Bottom left
		-20.0f,  20.0f, 0.0f,	 0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,    //Top left
		 20.0f,  20.0f, 0.0f,	 0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,    //Top right
		 20.0f, -20.0f, 0.0f,	 1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f,    //Bottom right
	};

	static std::vector<unsigned int> indicesSnekPart =
	{
		//First quad							
		0, 2, 1,
		0, 3, 2,
	};

	//Scrolling texture
	static std::vector<float> verticesPill =
	{
		///Bottom right quad
		//Position			     //Colour			        //Texture co-ords
		-5.0f, -5.0f, 0.0f,	 1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 1.0f,    //Bottom left
		-5.0f,  5.0f, 0.0f,	 1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    //Top left
		 5.0f,  5.0f, 0.0f,	 1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f,    //Top right
		 5.0f, -5.0f, 0.0f,	 1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f,    //Bottom right
	};

	static std::vector<unsigned int> indicesPill =
	{
		//First quad							
		0, 2, 1,
		0, 3, 2,
	};

	//Scrolling texture
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
