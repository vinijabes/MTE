#pragma once

#include "Mesh.h"
#include "Material.h"

namespace Kinematics
{
	class Model
	{
	public:
		static Ref<Model> Load(const std::string& path);

		void PushMesh(Ref<Mesh> mesh) { m_Meshes.push_back(mesh); }
		std::vector<Ref<Mesh>> GetMeshes() { return m_Meshes; }

	private:
		std::vector<Ref<Mesh>> m_Meshes;
		std::vector<Ref<Material>> m_Materials;
	};
}