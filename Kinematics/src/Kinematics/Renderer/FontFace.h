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
	public:
		virtual Character LoadGlyph(char c) = 0;
		virtual void Load(uint32_t size) = 0;

		Character Get(char c) const { return m_Characters.at(c); }

		static Ref<FontFace> DEFAULT;

	protected:
		std::map<char, Character> m_Characters;
	};
}