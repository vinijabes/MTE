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

		void PushMaterial(Ref<Material> material) { m_Materials.push_back(material); }
		int Bind(int slot = 0) 
		{
			int registered = 0;
			for (auto m : m_Materials)
			{
				registered += m->Bind(slot + registered);
			}

			return registered;
		}
	private:
		std::vector<Ref<Mesh>> m_Meshes;
		std::vector<Ref<Material>> m_Materials;
	};
}