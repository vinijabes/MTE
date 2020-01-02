#include "mtepch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Kinematics {

	int Texture::GetWrapping(WrappingOption opt)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: KINEMATICS_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return 0;
			case RendererAPI::API::OpenGL:
				if (opt == WrappingOption::KINEMATICS_CLAMP_TO_EDGE) return GL_CLAMP_TO_EDGE;
				if (opt == WrappingOption::KINEMATICS_CLAMP_TO_BORDER) return GL_CLAMP_TO_BORDER;
				if (opt == WrappingOption::KINEMATICS_REPEAT) return GL_REPEAT;
				return 0;
		}

		KINEMATICS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return 0;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	KINEMATICS_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(width, height);
		}

		KINEMATICS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, WrappingOption s, WrappingOption t)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	KINEMATICS_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(path, GetWrapping(s), GetWrapping(t));
		}

		KINEMATICS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}