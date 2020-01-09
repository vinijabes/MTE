#pragma once

#include "RenderCommand.h"

#include "Camera.h"
#include "Shader.h"
#include "Model.h"

namespace Kinematics {

	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Mesh>& mesh, const Ref<Shader>& shader = nullptr, uint32_t amount = 1);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> m_SceneData;
	};
}