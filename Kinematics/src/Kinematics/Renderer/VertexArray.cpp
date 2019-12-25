#include "mtepch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Kinematics {
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	KINEMATICS_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexArray>();
		}

		KINEMATICS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}