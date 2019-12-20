#pragma once

#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include "Kinematics/Framework/Events/Event.h"
#include "Kinematics/Core/Hash.h"

namespace Kinematics {


	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {}
		
		inline unsigned int GetWidth() const { return m_Width; };
		inline unsigned int GetHeight() const { return m_Height; };

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		unsigned int m_Width;
		unsigned int m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	typedef std::function<void(Event&)> WindowInputCallback;
	class WindowSubSystemInterface : public SubSystemInterface
	{
	public:
		virtual void SetWindowCloseCallback(WindowInputCallback fun) = 0;
		virtual void SetWindowResizeCallback(WindowInputCallback fun) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		SUBSYSTEM_CLASS_TYPE(WindowSubSystem);
	};
}