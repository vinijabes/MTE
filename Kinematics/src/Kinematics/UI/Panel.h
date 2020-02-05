#pragma once

#include "UIElementInterface.h"

namespace Kinematics
{
	namespace UI
	{
		class Panel : public UIElementInterface
		{
		public:
			virtual void Draw(Camera& camera, glm::vec2 pos = glm::vec2(0)) override;
		};
	}
}