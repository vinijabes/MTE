#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Kinematics {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawInstanced(const Ref<VertexArray>& vertexArray, uint32_t amount) = 0;

		virtual void EnableDepthTest() = 0;
		virtual void DisableDepthTest() = 0;

		virtual void EnableByteAlignment() = 0;
		virtual void DisableByteAlignment() = 0;

		virtual void EnableAlphaBlending() = 0;
		virtual void DisableAlphaBlending() = 0;

		virtual void EnableScissorTest() = 0;
		virtual void DisableScissorTest() = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}