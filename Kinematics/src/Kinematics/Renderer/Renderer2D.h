#pragma once

#include "Camera.h"

#include "Texture.h"
#include "Sprite.h"

#include "Text.h"
#include "FontFace.h"

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

		static void RenderText(const glm::vec2& position, Ref<Text> text);
		static void RenderText(const glm::vec3& position, Ref<Text> text);

		static void RenderChar(const glm::vec2& position, const glm::vec2& size, const Character& c, const glm::vec4& color);
	};
}