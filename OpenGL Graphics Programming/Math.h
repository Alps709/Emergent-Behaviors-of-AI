#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

namespace Math
{
	static glm::mat4 Create2DModelMatrix(float _translateX, float _translateY, float _rotationAngle, float _scaleX, float _scaleY)
	{
		//Translation matrix
		const glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(_translateX, _translateY, 0.0f));

		//Rotation matrix
		const glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(), glm::radians(_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

		//Scale matrix
		const glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(_scaleX, _scaleY, 1.0f));

		//Return model matrix
		return translationMatrix * rotationMatrixZ * scaleMatrix;
	}

	inline void LimitVector2D(glm::vec2& _vec, float _maxVal)
	{
		if(glm::length(_vec) > _maxVal)
		{
			_vec = glm::normalize(_vec) * _maxVal;
		}
	}
}
