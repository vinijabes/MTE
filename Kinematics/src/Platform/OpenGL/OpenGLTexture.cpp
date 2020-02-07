#include "mtepch.h"
#include "OpenGLTexture.h"
#include "Kinematics/Framework/Managers/EnviromentManager.h"

#include "stb_image.h"

namespace Kinematics {

	int ConvertToOpenGLFormat(DataType format)
	{
		switch (format)
		{
		case Kinematics::DataType::RGBA:
			return GL_RGBA;
			break;
		case Kinematics::DataType::RGBA8:
			return GL_RGBA8;
			break;
		case Kinematics::DataType::RED:
			return GL_RED;
			break;
		default:
			break;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, GLint s, GLint t)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		GenerateTexture(width, height, s, t);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, DataType format, GLint s, GLint t)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = ConvertToOpenGLFormat(format);
		m_DataFormat = ConvertToOpenGLFormat(format);

		GenerateTexture(width, height, s, t);
	}

	void OpenGLTexture2D::GenerateTexture(uint32_t width, uint32_t height, GLint s, GLint t)
	{
		if (EnviromentManager::Get(KINEMATICS_OPENGL_MAJOR) == 4 && EnviromentManager::Get(KINEMATICS_OPENGL_MINOR) == 5)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, s);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, t);
		}
		else
		{
			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);

			glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, NULL);
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, GLint s, GLint t)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		KINEMATICS_CORE_ASSERT(data, "Failed to Load Image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		KINEMATICS_CORE_ASSERT(internalFormat && dataFormat, "Format not supported!");

		if (EnviromentManager::Get(KINEMATICS_OPENGL_MAJOR) == 4 && EnviromentManager::Get(KINEMATICS_OPENGL_MINOR) >= 5)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, s);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, t);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
			
			glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
			
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		}

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : m_DataFormat == GL_RGB8 ? 3 : 1;
		KINEMATICS_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");

		if (EnviromentManager::Get(KINEMATICS_OPENGL_MAJOR) == 4 && EnviromentManager::Get(KINEMATICS_OPENGL_MINOR) >= 5)
		{
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		if (EnviromentManager::Get(KINEMATICS_OPENGL_MAJOR) == 4 && EnviromentManager::Get(KINEMATICS_OPENGL_MINOR) >= 5)
		{
			glBindTextureUnit(slot, m_RendererID);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
		}
	}
	void OpenGLTexture2D::SetMagFilter(TextureFilter filter)
	{
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, filter == TextureFilter::LINEAR ? GL_LINEAR : GL_NEAREST );
	}
	void OpenGLTexture2D::SetMinFilter(TextureFilter filter)
	{
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, filter == TextureFilter::LINEAR ? GL_LINEAR : GL_NEAREST);
	}
}