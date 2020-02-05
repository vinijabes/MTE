#pragma once

#include "Texture.h"
#include "Kinematics/Core/Core.h"

namespace Kinematics {
	class Text
	{
	public:

		Ref<Texture2D> GetTexture() { return m_TextTexture; }
	private:
		Ref<Texture2D> m_TextTexture;
		std::string m_Text;
	};
}