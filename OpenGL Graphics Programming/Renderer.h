#pragma once

#include "Object.h"
#include "Shader.h"
#include <unordered_map>


class Camera;

class Renderer
{
public:
	Renderer(Shader* _shader);
	~Renderer();

	void Render(const std::unordered_map<Mesh, std::vector<Object>>& _batches, const Camera& _myCamera);

	static void Clear();

private:
	Shader* m_Shader;
};

