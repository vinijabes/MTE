
#include "mtepch.h"
#include "OpenGLBuffer.h"
#include "Kinematics/Framework/Managers/EnviromentManager.h"

#include <glad/glad.h>
#include "OpenGLTexture.h"

namespace Kinematics {

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		if (EnviromentManager::Get(KINEMATICS_OPENGL_MAJOR) == 4 && EnviromentManager::Get(KINEMATICS_OPENGL_MINOR) >= 5)
		{
			glCreateBuffers(1, &m_RendererID);
		}
		else
		{
			glGenBuffers(1, &m_RendererID);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		if (EnviromentManager::Get(KINEMATICS_OPENGL_MAJOR) == 4 && EnviromentManager::Get(KINEMATICS_OPENGL_MINOR) >= 5)
		{
			glCreateBuffers(1, &m_RendererID);
		}
		else
		{
			glGenBuffers(1, &m_RendererID);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer()
	{
		if (EnviromentManager::Get(KINEMATICS_OPENGL_MAJOR) == 4 && EnviromentManager::Get(KINEMATICS_OPENGL_MINOR) >= 5)
		{
			glCreateFramebuffers(1, &m_RendererID);
		}
		else
		{
			glGenFramebuffers(1, &m_RendererID);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		Unbind();
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}
	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::SetTargetTexture(Ref<Texture2D> target)
	{
		auto targetId = std::static_pointer_cast<OpenGLTexture2D>(target)->GetRendereID();

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, targetId, 0);

		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
	}
}