#include "mtepch.h"
#include "Script.h"

#include "LuaScript.h"

namespace Kinematics {
	Ref<Script> Script::Create(std::string path)
	{
		return std::static_pointer_cast<Script>(LuaScript::Create(path));
	}
}