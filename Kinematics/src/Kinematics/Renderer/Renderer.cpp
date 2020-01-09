#include "mtepch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"

namespace Kinematics {

	Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Ref<Mesh>& mesh, const Ref<Shader>& shader, uint32_t amount)
	{	
		if (shader)
		{
			shader->Bind();
			shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		}

		mesh->Bind();
		if (amount == 1)
			RenderCommand::DrawIndexed(mesh->GetVertexArray());
		else
			RenderCommand::DrawInstanced(mesh->GetVertexArray(), amount);
	}
}