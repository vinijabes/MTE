#pragma once

#include "Kinematics/Renderer/RendererAPI.h"

namespace Kinematics {
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawInstanced(const Ref<VertexArray>& vertexArray, uint32_t amount) override;
	};
}