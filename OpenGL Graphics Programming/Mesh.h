#pragma once
#include "VertexBufferInfo.h"


class Mesh
{
public:
	Mesh() = default;
	Mesh(std::vector<float> _vertices, std::vector<unsigned int> _indices);
	~Mesh();

	//A comparsion is required for the hashing function below
	bool operator==(const Mesh &other) const
	{
		return (m_vaoID == other.m_vaoID
			&& m_vboID == other.m_vboID
			&& m_iboID == other.m_iboID);
	}

	void AddVAOBuffer(const VertexBufferInfo _info);
	void SetVertexAttributes();

	std::vector<float> m_vertices;
	unsigned int GetindicesCount() const { return m_indicesCount; }

	void Bind() const;
	static void Unbind();

	GLuint m_vaoID{0};
	GLuint m_vboID{0};
	GLuint m_iboID{0};

	//Used by the renderer to know how many indices to draw
	unsigned int m_indicesCount{0};
};

namespace std {
	//Hashing function for when I want the Mesh class to be able to be used as a key in an unordered_map
	template <>
	struct std::hash<Mesh>
	{
		std::size_t operator()(const Mesh& _mesh) const noexcept
		{
			//Compute individual hash values for first,
			//second and third and combine them using XOR
			//and bit shifting
			return ((std::hash<unsigned int>()(_mesh.m_vaoID)
				^ (std::hash<unsigned int>()(_mesh.m_vboID) << 1)) >> 1)
				^ (std::hash<unsigned int>()(_mesh.m_iboID) << 1);
		}
	};
}