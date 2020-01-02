#pragma once

#include "Kinematics/Core/Core.h"
#include "Kinematics/Core/Timestep.h"
#include "Kinematics/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Kinematics
{
	class Sprite
	{
	public:
		Sprite(Ref<Texture2D> texture, uint8_t frameCount, float animationTime) : m_Texture(texture), m_FrameCount(frameCount), m_RemainingTime(0.0f), m_CurrentFrame(0)
		{
			m_Size = glm::vec2(texture->GetWidth() / frameCount, texture->GetHeight());
			m_FrameTime = animationTime / frameCount;
		};

		Ref<Texture2D> GetTexture() const { return m_Texture; }
		virtual uint8_t GetCurrentFrame() const { return m_CurrentFrame; }

		virtual void OnUpdate(Timestep ts) 
		{ 
			m_RemainingTime -= ts.GetMilliseconds(); 
			if (m_RemainingTime < 0)
			{
				++m_CurrentFrame;
				m_RemainingTime = m_FrameTime;
				if (m_CurrentFrame >= m_FrameCount) m_CurrentFrame = 0;
			}
		}

		glm::vec2 GetSize() const { return m_Size; }

	private:
		Ref<Texture2D> m_Texture;

		glm::vec2 m_Size;
		uint8_t m_FrameCount;
		float m_FrameTime;

		float m_RemainingTime;
		uint8_t m_CurrentFrame;
	};
}