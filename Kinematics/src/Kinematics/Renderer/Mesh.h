#pragma once

#include "Kinematics/Core/Core.h"
#include "Kinematics/Core/Log.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Material.h"

namespace Kinematics
{
	class Mesh
	{
	public:
		void Bind() const { m_VertexArray->Bind(); }
		void Unbind() const { m_VertexArray->Unbind(); }

		static Ref<Mesh> Create()
		{
			Ref<Mesh> mesh = CreateRef<Mesh>();
			mesh->m_VertexArray = VertexArray::Create();
			return mesh;
		}

		static Ref<Mesh> Create(Ref<VertexBuffer> positionVB, Ref<VertexBuffer> normalVB, Ref<VertexBuffer> textureVB, Ref<IndexBuffer> indexBuffer)
		{
			Ref<Mesh> mesh = CreateRef<Mesh>();
			mesh->m_VertexArray = VertexArray::Create();
			mesh->m_VertexArray->AddVertexBuffer(positionVB);
			mesh->m_VertexArray->AddVertexBuffer(normalVB);
			mesh->m_VertexArray->AddVertexBuffer(textureVB);
			mesh->m_VertexArray->SetIndexBuffer(indexBuffer);
			mesh->m_PositionVertexBuffer = positionVB;
			mesh->m_TextureCoordinatesVertexBuffer = textureVB;
			mesh->m_IndexBuffer = indexBuffer;
		}

		void SetVertices(const std::vector<float> vertices)
		{
			m_Vertices = vertices;
			SetPositionVertexBuffer(&m_Vertices[0], sizeof(float) * m_Vertices.size());
		}

		void SetNormals(const std::vector<float> normals)
		{
			m_Normals = normals;
			SetNormalVertexBuffer(&m_Normals[0], sizeof(float) * m_Normals.size());
		}

		void SetTextureCoords(const std::vector<float> tc, bool includeMaterial = false)
		{
			m_TextureCoords = tc;
			SetTextureVertexBuffer(&m_TextureCoords[0], sizeof(float) * m_TextureCoords.size(), includeMaterial);
		}

		void SetIndices(const std::vector<uint32_t> indices)
		{
			m_Indices = indices;
			SetIndexBuffer(&m_Indices[0], m_Indices.size());
		}

		void SetPositionVertexBuffer(float* vertices, uint32_t size)
		{
			if (m_PositionVertexBuffer != nullptr) KINEMATICS_CORE_WARN("OVERWRITING VERTEX ARRAY BUFFER");
			m_PositionVertexBuffer.reset(VertexBuffer::Create(vertices, size));
			m_PositionVertexBuffer->SetLayout({ {ShaderDataType::Float3, "a_Position"} });
			m_VertexArray->AddVertexBuffer(m_PositionVertexBuffer, 0);
		}

		void SetNormalVertexBuffer(float* vertices, uint32_t size)
		{
			if (m_NormalVertexBuffer != nullptr) KINEMATICS_CORE_WARN("OVERWRITING VERTEX ARRAY BUFFER");
			m_NormalVertexBuffer.reset(VertexBuffer::Create(vertices, size));
			m_NormalVertexBuffer->SetLayout({ {ShaderDataType::Float3, "a_Normal"} });
			m_VertexArray->AddVertexBuffer(m_NormalVertexBuffer, 1);
		}

		void SetTextureVertexBuffer(float* vertices, uint32_t size, bool includeMaterial)
		{
			if (m_TextureCoordinatesVertexBuffer != nullptr) KINEMATICS_CORE_WARN("OVERWRITING VERTEX ARRAY BUFFER");
			m_TextureCoordinatesVertexBuffer.reset(VertexBuffer::Create(vertices, size));

			if(includeMaterial)
				m_TextureCoordinatesVertexBuffer->SetLayout({ {ShaderDataType::Float3, "a_TexCoord"} });
			else
				m_TextureCoordinatesVertexBuffer->SetLayout({ {ShaderDataType::Float2, "a_TexCoord"} });
			m_VertexArray->AddVertexBuffer(m_TextureCoordinatesVertexBuffer, 2);
		}

		void SetIndexBuffer(uint32_t* indices, uint32_t count)
		{
			m_IndexBuffer.reset(IndexBuffer::Create(indices, count));
			m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		}

		void SetBuffer(Ref<VertexBuffer> buffer, uint32_t pos, uint32_t divisor = 0)
		{
			if (pos < 2) KINEMATICS_CORE_WARN("Mesh::SetBuffer: OVERWRITING MESH VERTEX ARRAY BUFFER ({})", pos);
			m_VertexArray->AddVertexBuffer(buffer, pos, divisor);
		}
		
		void PushMaterial(Ref<Material> material)
		{
			m_Materials.push_back(material);
		}

		Ref<VertexArray> GetVertexArray() { return m_VertexArray; }
		Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_PositionVertexBuffer;
		Ref<VertexBuffer> m_NormalVertexBuffer;
		Ref<VertexBuffer> m_TextureCoordinatesVertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		
		std::vector<float> m_Vertices;
		std::vector<float> m_Normals;
		std::vector<float> m_TextureCoords;
		std::vector<uint32_t> m_Indices;

		std::vector<Ref<Material>> m_Materials;
	};
}