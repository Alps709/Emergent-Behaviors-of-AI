#include "Camera.h"
#include "Math.h"
#include "Renderer.h"
#include <freeglut.h>

Renderer::Renderer(Shader* _shader)
{
	m_Shader = _shader;
}

Renderer::~Renderer()
= default;

void Renderer::Render(const std::unordered_map<Mesh, std::vector<Object>>& _batches, const Camera& _myCamera)
{
	//Prepare renderer (eg. clear buffer, create PVM matrix etc.)
	glm::mat4 projViewMat = _myCamera.GetProjView();
	
	for(auto& batch: _batches)
	{
		//Get the mesh used for this batch
		const Mesh& mesh = batch.first;
		
		//Bind the mesh that all the models will use
		mesh.Bind();

		//Iterate over all models in the batch
		for(auto objectIt = batch.second.rbegin(); objectIt != batch.second.rend(); ++objectIt)
		{
			glm::mat4 pvmMat = projViewMat * objectIt->GetModelMat();

			//Prepare the object for drawing
			objectIt->BindTexture(0);

			//Set object specific uniforms
			m_Shader->SetUniform1i("tex1", 0);
			m_Shader->SetUniformMat4f("u_PVM", pvmMat);
			
			//Draw the object
			GLCall(glDrawElements(GL_TRIANGLES, mesh.GetindicesCount(), GL_UNSIGNED_INT, static_cast<void *>(0)));
		}
		mesh.Unbind();
	}
}

void Renderer::Clear()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
