#include "mtepch.h"
#include "Panel.h"


namespace Kinematics {
	namespace UI {
		void Panel::Draw(Camera& camera, glm::vec2 parentPos)
		{
			DrawChildren(camera, m_Position);
		}
	}
}