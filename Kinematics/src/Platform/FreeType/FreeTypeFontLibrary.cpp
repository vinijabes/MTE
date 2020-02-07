#include "mtepch.h"
#include "FreeTypeFontLibrary.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "FreeTypeFontFace.h"

namespace Kinematics
{
	FreeTypeFontLibrary::FreeTypeFontLibrary()
		: m_Library(nullptr)
	{
		if (FT_Init_FreeType((FT_Library*)&m_Library))
			KINEMATICS_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
	}

	FreeTypeFontLibrary::~FreeTypeFontLibrary()
	{
		FT_Done_FreeType((FT_Library)m_Library);
	}

	Ref<FontFace> FreeTypeFontLibrary::CreateFontFace(std::string path)
	{
		return CreateRef<FreeTypeFontFace>(GetPtr(), path);
	}
}