#pragma once

#include "Texture.h"
#include "Kinematics/Core/Core.h"

#include <map>
#include <glm/glm.hpp>

namespace Kinematics
{
	struct Character
	{
		Ref<Texture2D> Texture;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		int Advance;
	};

	class FontFace
	{

	private:
		std::map<char, Character> m_Characters;
	};
}