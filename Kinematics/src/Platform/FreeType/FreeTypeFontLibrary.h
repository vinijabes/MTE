#pragma once

#include "Kinematics/Renderer/FontLibrary.h"
#include "Kinematics/Renderer/FontFace.h"

namespace Kinematics
{
	struct FT_LibraryRec_;

	class FreeTypeFontLibrary : public FontLibrary, public std::enable_shared_from_this<FreeTypeFontLibrary>
	{
	public:
		FreeTypeFontLibrary();
		~FreeTypeFontLibrary();

		virtual Ref<FontFace> CreateFontFace(std::string path) override;
		FT_LibraryRec_* GetRaw() { return m_Library; }

		Ref<FreeTypeFontLibrary> GetPtr()
		{
			return shared_from_this();
		}
	private:
		FT_LibraryRec_* m_Library;
	};
}