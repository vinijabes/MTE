#pragma once

#include "Shader.h"
#include "Material.h"

#include "Kinematics/Core/Core.h"

namespace Kinematics
{
	class BatchContainer
	{
	public:

	private:
		Ref<Shader> m_Shader;
		std::list<Ref<Material>> m_Materials;
	};
}