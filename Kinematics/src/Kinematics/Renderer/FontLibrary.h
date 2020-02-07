#pragma once

#include "FontFace.h"

namespace Kinematics
{
	class FontLibrary
	{
	public:
		static Ref<FontLibrary> Create();

		virtual Ref<FontFace> CreateFontFace(std::string path) = 0;
	};
}