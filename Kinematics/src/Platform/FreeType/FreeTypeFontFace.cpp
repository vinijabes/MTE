#include "mtepch.h"
#include "FreeTypeFontFace.h"

#include "FreeTypeFontLibrary.h"
#include "Kinematics/Renderer/RenderCommand.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

namespace Kinematics
{
	FreeTypeFontFace::FreeTypeFontFace(Ref<FontLibrary> library, const std::string& path)
		: m_Library(library), m_Path(path)
	{
		auto freetypelib = std::static_pointer_cast<FreeTypeFontLibrary>(m_Library);
		if (FT_New_Face((FT_Library)freetypelib->GetRaw(), m_Path.c_str(), 0, (FT_Face*)&m_Face))
			KINEMATICS_ERROR("ERROR::FREETYPE: Failed to load font");

		Load(48);
	}

	FreeTypeFontFace::~FreeTypeFontFace()
	{
		FT_Done_Face((FT_Face)m_Face);
	}

	void FreeTypeFontFace::Load(uint32_t size)
	{
		FT_Set_Pixel_Sizes((FT_Face)m_Face, 0, size);
		auto face = ((FT_Face)m_Face);
		Kinematics::RenderCommand::DisableByteAlignment();

		m_MaxBearing = 0;

		m_MaxSize = glm::ivec2(0);
		m_MaxSize = glm::ivec2((face->bbox.xMax - face->bbox.xMin) >> 6, (face->size->metrics.ascender - face->size->metrics.descender) >> 6);
		for (unsigned int c = 0; c < 256; c++)
		{
			m_Characters[c] = LoadGlyph(c);
			if (m_Characters[c].Bearing.y > m_MaxBearing) m_MaxBearing = m_Characters[c].Bearing.y;
		}

		Kinematics::RenderCommand::EnableByteAlignment();
	}

	Character FreeTypeFontFace::LoadGlyph(uint32_t c)
	{
		auto face = (FT_Face)m_Face;

		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
			KINEMATICS_ERROR("ERROR::FREETYTPE: Failed to load Glyph");

		auto texture = Texture2D::Create(face->glyph->bitmap.width, face->glyph->bitmap.rows, DataType::RED, WrappingOption::KINEMATICS_CLAMP_TO_EDGE, WrappingOption::KINEMATICS_CLAMP_TO_EDGE);
		
		texture->SetMinFilter(TextureFilter::LINEAR);
		texture->SetMagFilter(TextureFilter::LINEAR);
		
		texture->SetData(face->glyph->bitmap.buffer, face->glyph->bitmap.width * face->glyph->bitmap.rows * 1);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x,
			c
		};

		return character;
	}
	uint32_t FreeTypeFontFace::GetAscender() const
	{
		return ((FT_Face)m_Face)->size->metrics.ascender >> 6;
	}
	uint32_t FreeTypeFontFace::GetDescender() const
	{
		return std::abs(((FT_Face)m_Face)->size->metrics.descender) >> 6;
	}
	uint32_t FreeTypeFontFace::GetHeight() const
	{
		return ((FT_Face)m_Face)->size->metrics.height >> 6;
	}
}