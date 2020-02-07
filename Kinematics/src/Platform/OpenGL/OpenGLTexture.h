#pragma once

#include "Kinematics/Renderer/Texture.h"

#include <glad/glad.h>

namespace Kinematics {
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, GLint s, GLint t);
		OpenGLTexture2D(uint32_t width, uint32_t height, DataType format, GLint s, GLint t);
		OpenGLTexture2D(const std::string& path, GLint s, GLint t);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual void SetMagFilter(TextureFilter filter) override;
		virtual void SetMinFilter(TextureFilter filter) override;

		uint32_t GetRendereID() const { return m_RendererID; }

	protected:
		void GenerateTexture(uint32_t width, uint32_t height, GLint s, GLint t);
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}