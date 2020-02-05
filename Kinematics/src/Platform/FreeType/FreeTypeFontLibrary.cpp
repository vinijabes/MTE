#include "mtepch.h"
#include "FreeTypeFontLibrary.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

namespace Kinematics
{
	FreeTypeFontLibrary::FreeTypeFontLibrary()
		: m_Library(nullptr)
	{
		if (FT_Init_FreeType((FT_Library*)&m_Library))
			KINEMATICS_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
	}
}