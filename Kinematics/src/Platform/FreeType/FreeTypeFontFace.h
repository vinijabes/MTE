#pragma once

#include "Kinematics/Renderer/FontFace.h"
#include "Kinematics/Renderer/FontLibrary.h"


namespace Kinematics {

	struct FT_LibraryRec_;

	class FreeTypeFontFace : public FontFace
	{
	public:
		FreeTypeFontFace(Ref<FontLibrary> library, const std::string& path);
		~FreeTypeFontFace();

		virtual void Load(uint32_t size) override;

		virtual Character LoadGlyph(char c) override;
	private:
		Ref<FontLibrary> m_Library;
		FT_LibraryRec_* m_Face;		

		std::string m_Path;
	};
}