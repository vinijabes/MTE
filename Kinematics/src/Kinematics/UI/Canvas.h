#pragma once

#include "UIElementInterface.h"

namespace Kinematics
{
	class Canvas : public UIElementInterface
	{
	public:
		virtual void Draw(Camera& camera, glm::vec2 parentPos = glm::vec2(0)) override
		{
			auto pos = m_Position;
			auto size = m_Size;
			auto drawingPos = pos + size / 2.f + parentPos;

			Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size), glm::vec4(1.0f));
			DrawChildren(camera, pos);
		}

		virtual void OnChange() override
		{

		}

		virtual void Update(Timestep ts) override
		{

		}

	private:

	};
}