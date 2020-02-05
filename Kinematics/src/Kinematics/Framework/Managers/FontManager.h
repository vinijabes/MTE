#pragma once

#include "Kinematics/Core/Core.h"
#include "Kinematics/Renderer/FontLibrary.h"

namespace Kinematics {
	class FT_Library;

	class FontManager
	{
	public:
		FontManager(FontManager const&) = delete;
		void operator=(FontManager const&) = delete;

		static FontManager* GetInstance();


	private:
		FontManager() {}
	private:
		static FontManager* m_Instance;
		static Ref<FontLibrary> m_Library;
	};
}