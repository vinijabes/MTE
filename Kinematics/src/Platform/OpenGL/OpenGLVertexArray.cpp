#include "mtepch.h"
#include "OpenGLVertexArray.h"
#include "Kinematics/Framework/Managers/EnviromentManager.h"

#include <glad/glad.h>

namespace Kinematics {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Kinematics::ShaderDataType::Float:    return GL_FLOAT;
			case Kinematics::ShaderDataType::Float2:   return GL_FLOAT;
			case Kinematics::ShaderDataType::Float3:	return GL_FLOAT;
			case Kinematics::ShaderDataType::Float4:	return GL_FLOAT;
			case Kinematics::ShaderDataType::Mat3:     return GL_FLOAT;
			case Kinematics::ShaderDataType::Mat4:     return GL_FLOAT;
			case Kinematics::ShaderDataType::Int:      return GL_INT;
			case Kinematics::ShaderDataType::Int2:     return GL_INT;
			case Kinematics::ShaderDataType::Int3:     return GL_INT;
			case Kinematics::ShaderDataType::Int4:     return GL_INT;
			case Kinematics::ShaderDataType::Bool:     return GL_BOOL;
		}

		KINEMATICS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		if (EnviromentManager::Get(KINEMATICS_OPENGL_MAJOR) == 4 && EnviromentManager::Get(KINEMATICS_OPENGL_MINOR) == 5)
		{
			glCreateVertexArrays(1, &m_RendererID);
		}
		else
		{
			glGenVertexArrays(1, &m_RendererID);
			glBindVertexArray(m_RendererID);
		}
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer, uint32_t offset, uint32_t divisor)
	{
		KINEMATICS_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(offset);
			glVertexAttribPointer(offset,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			if (divisor)
				glVertexAttribDivisor(offset, divisor);

			++offset;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}