#pragma once

#include "RendererAPI.h"
#include "Kinematics/Framework/Interface/WindowSubSystemInterface.h"

namespace Kinematics {

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetScissor(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawInstanced(const Ref<VertexArray>& vertexArray, uint32_t amount)
		{
			s_RendererAPI->DrawInstanced(vertexArray, amount);
		}

		inline static void EnableDepthTest()
		{
			s_RendererAPI->EnableDepthTest();
		}

		inline static void DisableDepthTest()
		{
			s_RendererAPI->DisableDepthTest();
		}

		inline static void EnableByteAlignment()
		{
			s_RendererAPI->EnableByteAlignment();
		}

		inline static void DisableByteAlignment()
		{
			s_RendererAPI->DisableByteAlignment();
		}

		inline static void EnableAlphaBlending()
		{
			s_RendererAPI->EnableAlphaBlending();
		}

		inline static void DisableAlphaBlending()
		{
			s_RendererAPI->DisableAlphaBlending();
		}

		inline static void EnableScissorTest()
		{
			s_RendererAPI->EnableScissorTest();
		}

		inline static void DisableScissorTest()
		{
			s_RendererAPI->DisableScissorTest();
		}

		static WindowSubSystemInterface* GetWindow() { return m_Window; }
		static void SetWindow(WindowSubSystemInterface* window) { m_Window = window; }

	private:
		static Scope<RendererAPI> s_RendererAPI;
		static WindowSubSystemInterface* m_Window;
	};
}