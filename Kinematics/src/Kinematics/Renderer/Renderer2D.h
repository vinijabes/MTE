#pragma once

#include "Camera.h"

#include "Texture.h"
#include "Sprite.h"

namespace Kinematics {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& srcRect);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& srcRect);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Sprite>& sprite);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Sprite>& sprite);

	};
}