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
		unsigned int UCode;
	};

	class FontFace
	{
	public:
		FontFace()
			: m_MaxBearing(0), m_MaxSize(0)
		{}
		
		virtual Character LoadGlyph(uint32_t c) = 0;
		virtual void Load(uint32_t size) = 0;

		Character Get(uint32_t c) const
		{
			return m_Characters.at(c); 
		}
		uint32_t GetMaxVerticalBearing() const { return m_MaxBearing; }
		
		virtual uint32_t GetAscender() const = 0;
		virtual uint32_t GetDescender() const = 0;
		virtual uint32_t GetHeight() const = 0;

		glm::ivec2 GetMaxSize() const { return m_MaxSize; }

		static Ref<FontFace> DEFAULT;

	protected:
		std::map<uint32_t, Character> m_Characters;

		uint32_t m_MaxBearing;
		glm::ivec2 m_MaxSize;
	};
}