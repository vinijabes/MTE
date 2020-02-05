#pragma once

#include "Kinematics/Renderer/FontLibrary.h"

namespace Kinematics
{
	struct FT_LibraryRec_;

	class FreeTypeFontLibrary : public FontLibrary
	{
	public:
		FreeTypeFontLibrary();
	private:
		FT_LibraryRec_* m_Library;
	};
}