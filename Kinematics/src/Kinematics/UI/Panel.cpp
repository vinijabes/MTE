#include "mtepch.h"
#include "Panel.h"


namespace Kinematics {
	namespace UI {
		void Panel::Draw(Camera& camera, glm::vec2 parentPos)
		{
			if (m_Color != glm::vec4(0.f))
			{
				auto pos = m_Position;
				auto size = GetSize();
				auto drawingPos = pos + size / 2.f + parentPos;

				Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size), m_Color);
			} 
			else if (m_Theme)
			{
				auto pos = m_Position;
				auto size = GetSize();
				auto drawingPos = pos + size / 2.f + parentPos;

				Renderer2D::DrawQuad(camera.ToWindowPosition(drawingPos), camera.PixelToWindowSize(size), m_Theme->m_Color);
			}

			DrawChildren(camera, parentPos + m_Position);
		}
	}
}