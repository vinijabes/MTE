#include "mtepch.h"
#include "FontManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

namespace Kinematics
{
	FontManager* FontManager::m_Instance = NULL;
	Ref<FontLibrary> FontManager::m_Library = NULL;

	FontManager* FontManager::GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new FontManager();
			m_Library = FontLibrary::Create();
		}

		return m_Instance;
	}
}