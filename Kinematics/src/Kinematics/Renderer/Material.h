#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

namespace Kinematics
{
	enum class MaterialTypes : uint16_t
	{
		AMBIENT_TEXTURE,
		DIFFUSE_TEXTURE,
		ALPHA_TEXTURE,
		NORMAL_TEXTURE,
		SPECULAR_TEXTURE,
		SPECULAR_HIGHLIGHT_TEXTURE,
	};

	class Material
	{
	public:
		Material() = default;
		Material(float Ns, glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, glm::vec3 Ke, float Ni, float d, int16_t illum, glm::vec4 color)
			: m_Ns(Ns), m_Ka(Ka), m_Kd(Kd), m_Ks(Ks), m_Ke(Ke), m_Ni(Ni), m_D(d), m_Illum(illum) {}

		void PushTexture(MaterialTypes type, Ref<Texture2D> texture)
		{
			if (m_Textures.find(type) == m_Textures.end())
				m_Textures[type] = texture;
			else
				KINEMATICS_CORE_ERROR("Material::PushTexture: Material already has texture of type {} registered", int(type));
		}

		Ref<Texture2D> GetTexture(MaterialTypes type)
		{
			if (m_Textures.find(type) == m_Textures.end())
				return nullptr;
			return m_Textures[type];
		}

		int Bind(int slot)
		{
			int resources = 0;
			for (auto t : m_Textures)
			{
				t.second->Bind(slot + resources++);
			}
			return resources;
		}


		void SetNs(float ns) { m_Ns = ns; }
		void SetNi(float ni) { m_Ni = ni; }
		void SetD(float d) { m_D = d; }
		void SetIllum(int16_t illum) { m_Illum = illum; }

		void SetKa(glm::vec3 ka) { m_Ka = ka; }
		void SetKd(glm::vec3 kd) { m_Kd = kd; }
		void SetKs(glm::vec3 ks) { m_Ks = ks; }
		void SetKe(glm::vec3 ke) { m_Ke = ke; }

		float GetNs() const { return m_Ns; }
		float GetNi() const { return m_Ni; }
		float GetD() const { return m_D; }
		int16_t GetIllum() const { return m_Illum; }

		glm::vec3 GetKa() const { return m_Ka; }
		glm::vec3 GetKd() const { return m_Kd; }
		glm::vec3 GetKs() const { return m_Ks; }
		glm::vec3 GetKe() const { return m_Ke; }

	private:
		float m_Ns;
		float m_Ni;
		float m_D;
		int16_t m_Illum;

		glm::vec3 m_Ka;
		glm::vec3 m_Kd;
		glm::vec3 m_Ks;
		glm::vec3 m_Ke;

		std::unordered_map<MaterialTypes, Ref<Texture2D>> m_Textures;
	};
}