#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

namespace Kinematics
{
	enum MaterialTypes : uint16_t
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
		Material(float Ns, glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, glm::vec3 Ke, float Ni, float d, int16_t illum, glm::vec4 color) 
			: m_Ns(Ns), m_Ka(Ka), m_Kd(Kd), m_Ks(Ks), m_Ke(Ke), m_Ni(Ni), m_D(d), m_Illum(illum), m_Color(color){}

		void PushTexture(MaterialTypes type, Ref<Texture2D> texture)
		{
			if (m_Textures.find(type) == m_Textures.end())
				m_Textures[type] = texture;
			else
				KINEMATICS_CORE_ERROR("Material::PushTexture: Material already has texture of type {} registered", type);
		}

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
		glm::vec4 m_Color;
	};
}