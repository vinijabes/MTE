#include "mtepch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Kinematics {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	KINEMATICS_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLVertexBuffer(vertices, size);
		}

		KINEMATICS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	KINEMATICS_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLIndexBuffer(indices, size);
		}

		KINEMATICS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<FrameBuffer> FrameBuffer::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	KINEMATICS_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFrameBuffer>();
		}

		KINEMATICS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}