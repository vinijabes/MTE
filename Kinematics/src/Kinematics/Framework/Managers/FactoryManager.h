#pragma once

#include "Kinematics/Core/Core.h"
#include "Kinematics/Framework/Interface/SubSystemInterface.h"
#include <functional>
#include <unordered_map>

namespace Kinematics {
	class NetworkMessage;

	typedef std::function<Ref<SubSystemInterface>()> SUBSYSTEM_CONSTRUCTOR;
	typedef std::function<Ref<NetworkMessage>()> MESSAGE_CONSTRUCTOR;

	class FactoryManager
	{
	public:
		FactoryManager(FactoryManager const&) = delete;
		void operator=(FactoryManager const&) = delete;

		static FactoryManager* GetInstance()
		{
			if (!m_Instance)
			{
				m_Instance = new FactoryManager();
			}

			return m_Instance;
		}

		void RegisterSubSystem(std::string subsystem, SUBSYSTEM_CONSTRUCTOR subsystemFactory);
		Ref<SubSystemInterface> Create(std::string subsystem);

		void RegisterMessage(std::string subsystem, MESSAGE_CONSTRUCTOR messageFactory);
		Ref<NetworkMessage> CreateMessage(std::string message);
	private:
		FactoryManager() {}

	private:
		static FactoryManager* m_Instance;

		std::unordered_map<std::string, SUBSYSTEM_CONSTRUCTOR> m_FactoryTemplates;
		std::unordered_map<std::string, MESSAGE_CONSTRUCTOR> m_MessagesTemplates;
	};

	template <typename T>
	class FactoryRegistrator
	{
	public:
		FactoryRegistrator(const char* id)
		{
			FactoryManager::GetInstance()->RegisterSubSystem(id, []() {return std::static_pointer_cast<SubSystemInterface>(T::Create()); });
		}

		FactoryRegistrator(const char* id, SUBSYSTEM_CONSTRUCTOR fn)
		{
			FactoryManager::GetInstance()->RegisterSubSystem(id, fn);
		}
	};

	template <typename T>
	class MessageFactoryRegistrator
	{
	public:
		MessageFactoryRegistrator(const char* id)
		{
			FactoryManager::GetInstance()->RegisterMessage(id, []() {return std::static_pointer_cast<NetworkMessage>(CreateRef<T>()); });
		}

		MessageFactoryRegistrator(const char* id, MESSAGE_CONSTRUCTOR fn)
		{
			FactoryManager::GetInstance()->RegisterMessage(id, fn);
		}
	};

#define CREATE_FACTORY(id, type) inline extern FactoryRegistrator<type> _##type##Factory(id);
#define CREATE_MESSAGE_FACTORY(id, type) inline extern MessageFactoryRegistrator<type> _##type##Factory(id);
}