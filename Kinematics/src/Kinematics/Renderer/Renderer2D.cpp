#include "mtepch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Kinematics {
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		Ref<Shader> TextShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);

		s_Data->TextureShader->SetFloat4("u_TexCoords", { 0.0f, 0.0f, 1.0f, 1.0f });

		s_Data->TextShader = Shader::Create("assets/shaders/Text.glsl");
		s_Data->TextShader->Bind();
		s_Data->TextShader->SetFloat3("u_TextColor", glm::vec3(1.f));
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data->TextShader->Bind();
		s_Data->TextShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());	
	}

	void Renderer2D::EndScene()
	{
		s_Data->TextureShader->Unbind();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_TexCoords", { 0.0f, 0.0f, 1.0f, 1.0f });

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_TexCoords", { 0.0f, 0.0f, 1.0f, 1.0f });

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& srcRect)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, srcRect);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& srcRect)
	{
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		glm::vec4 texCoords(srcRect.x/texture->GetWidth(), srcRect.y/texture->GetHeight(), srcRect.z / texture->GetWidth(), srcRect.w/texture->GetHeight());

		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_TexCoords", texCoords);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Sprite>& sprite)
	{
		auto rectSize = sprite->GetSize();
		auto currentFrame = sprite->GetCurrentFrame();
		DrawQuad({ position.x, position.y, 0.0f }, size, sprite->GetTexture(), { rectSize.x * currentFrame, 0.0f, rectSize.x * (currentFrame + 1), rectSize.y});
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Sprite>& sprite)
	{
		auto rectSize = sprite->GetSize();
		auto currentFrame = sprite->GetCurrentFrame();
		DrawQuad(position, size, sprite->GetTexture(), { rectSize.x * currentFrame, 0.0f, rectSize.x * (currentFrame + 1), rectSize.y });
	}

	void Renderer2D::RenderChar(const glm::vec2& position, const glm::vec2& size, const Character& c, const glm::vec4& color)
	{
		s_Data->TextShader->Bind();
		c.Texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextShader->SetMat4("u_Transform", transform);
		s_Data->TextShader->SetFloat4("u_TexCoords", { 0.0f, 0.0f, 1.0f, 1.0f });
		s_Data->TextShader->SetFloat3("u_TextColor", glm::vec3(color));


		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
		s_Data->TextureShader->Bind();
	}
}