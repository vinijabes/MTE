#include "mtepch.h"
#include "FontLibrary.h"

#include "Platform/FreeType/FreeTypeFontLibrary.h"

namespace Kinematics
{
	Ref<FontLibrary> FontLibrary::Create()
	{
		return CreateRef<FreeTypeFontLibrary>();
	}
}