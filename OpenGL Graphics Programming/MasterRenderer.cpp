
#include "Math.h"
#include "Renderer.h"

#include "MasterRenderer.h"

MasterRenderer::MasterRenderer()
{
	m_shader = new Shader();
	m_renderer = new Renderer(m_shader);
}


MasterRenderer::~MasterRenderer()
{
	delete m_shader;
	delete m_renderer;
}

void MasterRenderer::Render(const Camera& _myCamera) const
{
	//Bind shader
	m_shader->Bind();

	//Pass any default uniforms (that are shared across all shaders) into the shader here

	//Render batches
	m_renderer->Render(m_batches, _myCamera);

	//Unbind shader
	Shader::Unbind();
}

void MasterRenderer::ProcessObject(Object _object)
{
	Mesh tempMesh = *(_object.GetMesh());

	std::vector<Object>& batch = m_batches[tempMesh];

	if(batch.empty())
	{
		std::vector<Object> tempVec;
		tempVec.push_back(_object);
		batch = tempVec;
	}
	else
	{
		batch.push_back(_object);
	}

}

void MasterRenderer::ProcessObject(std::vector<Object> _objects)
{
	for (auto & object : _objects)
	{
		Mesh tempMesh = *(object.GetMesh());

		std::vector<Object>& batch = m_batches[tempMesh];

		if (batch.empty())
		{
			std::vector<Object> tempVec;
			tempVec.push_back(object);
			batch = tempVec;
		}
		else
		{
			batch.push_back(object);
		}
	}
}