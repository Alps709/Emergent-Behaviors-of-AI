#pragma once
#include <unordered_map>

#include "Object.h"

#include "Camera.h"

class Renderer;
class Shader;

class MasterRenderer
{
public:
	MasterRenderer();
	~MasterRenderer();

	void Render(const Camera& _myCamera) const;
	void ProcessObject(Object _object);
	void ProcessObject(std::vector<Object> _objects);

	//Batches for batch rendering
	std::unordered_map<Mesh, std::vector<Object>> m_batches;

private:
	Shader* m_shader = nullptr;
	Renderer* m_renderer = nullptr;
};

