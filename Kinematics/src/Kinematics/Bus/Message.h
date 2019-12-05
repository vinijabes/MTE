#pragma once

namespace Kinematics {
	class Message
	{
	public:
		Message(const std::string event)
		{
			m_MessageEvent = event;
		}

		std::string getEvent()
		{
			return m_MessageEvent;
		}
	private:
		std::string m_MessageEvent;
	};
}