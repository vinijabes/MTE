#pragma once

#include <string>

namespace Kinematics {

	enum WrappingOption
	{
		KINEMATICS_REPEAT,
		KINEMATICS_CLAMP_TO_EDGE,
		KINEMATICS_CLAMP_TO_BORDER,
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		static int GetWrapping(WrappingOption opt);

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path, WrappingOption s = WrappingOption::KINEMATICS_REPEAT, WrappingOption t = WrappingOption::KINEMATICS_REPEAT);
	};
}