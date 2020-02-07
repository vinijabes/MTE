#include "mtepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Kinematics {
	void OpenGLRendererAPI::Init()
	{
		//glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::DrawInstanced(const Ref<VertexArray>& vertexArray, uint32_t amount)
	{
		glDrawElementsInstanced(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, amount);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void OpenGLRendererAPI::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::EnableByteAlignment()
	{
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
	}

	void OpenGLRendererAPI::DisableByteAlignment()
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}
	
	void OpenGLRendererAPI::EnableAlphaBlending()
	{
		glEnable(GL_BLEND);
	}

	void OpenGLRendererAPI::DisableAlphaBlending()
	{
		glDisable(GL_BLEND);
	}
}