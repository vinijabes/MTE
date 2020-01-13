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

		void SetTextureVertexBuffer(float* vertices, uint32_t size)
		{
			if (m_TextureCoordinatesVertexBuffer != nullptr) KINEMATICS_CORE_WARN("OVERWRITING VERTEX ARRAY BUFFER");
			m_TextureCoordinatesVertexBuffer.reset(VertexBuffer::Create(vertices, size));
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
		
		void SetMaterial(Ref<Material> material)
		{
			m_Material = material;
		}

		Ref<VertexArray> GetVertexArray() { return m_VertexArray; }
		Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_PositionVertexBuffer;
		Ref<VertexBuffer> m_NormalVertexBuffer;
		Ref<VertexBuffer> m_TextureCoordinatesVertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<Material> m_Material;
	};
}