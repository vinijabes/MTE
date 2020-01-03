#pragma once


#include "Kinematics/Core/Core.h"

#include <unordered_map>

namespace Kinematics {

	class ResourceListInterface
	{
		friend class Resources;
	public:
		virtual ~ResourceListInterface() {}
	protected:
		ResourceListInterface() {}

		virtual size_t GetNumResources() = 0;
	};

	template <typename K, typename T>
	class ResourceList : public ResourceListInterface
	{
	friend class Resources;
	public:
		ResourceList() : ResourceListInterface() {};
		~ResourceList()
		{
			for(auto it = m_Resources.begin(); it != m_Resources.end(); it++)
				it->second.reset();
			m_Resources.clear();
		}

	protected:

		bool Add(K key, Ref<T> resource)
		{
			if (m_Resources.find(key) != m_Resources.end())
				return false;
			
			for (auto it = m_Resources.begin(); it != m_Resources.end(); ++it)
			{
				if (it->second == resource) return false;
			}

			m_Resources[key] = resource;
			return true;
		}

		bool Remove(K key)
		{
			auto it = m_Resources.find(key);
			if (it != m_Resources.end())
			{
				delete it->second;
				m_Resources.erase(it);
				return true;
			}
			return true;
		}

		Ref<T> Get(K key)
		{
			auto it = m_Resources.find(key);
			if (it == m_Resources.end()) return nullptr;
			return it->second;
		}

		size_t GetNumResources() { return m_Resources.size(); }


	private:
		std::unordered_map<K, Ref<T>> m_Resources;
	};


	class Resources
	{
	public:
		template <typename T>
		static bool Add(std::string key, Ref<T> resource)
		{
			if (!resource) return false;

			ResourceList<std::string, T>* resourceList = GetResourceList<T>();
			if (!resourceList)
			{
				size_t hash = typeid(T).hash_code();
				s_Instance.m_ResourceLists[hash] = CreateScope<ResourceList<std::string, T>>();
				resourceList = (ResourceList<std::string, T>*) s_Instance.m_ResourceLists[hash].get();
			}

			return resourceList->Add(key, resource);
		}

		template <typename T>
		static bool Remove(std::string key)
		{
			ResourceList<std::string, T>* resourceList = GetResourceList<T>();
			if (!resourceList) return false;
			return resourceList->Remove(key);
		}

		static void Flush()
		{
			for (auto it = s_Instance.m_ResourceLists.begin(); it != s_Instance.m_ResourceLists.end(); ++it)
				it->second.reset();
			s_Instance.m_ResourceLists.clear();
		}

		template<typename T>
		static Ref<T> Get(std::string key)
		{
			// return early if the asset list isn't the type map
			ResourceList<std::string, T>* resourceList = GetResourceList<T>();
			if (!resourceList) return nullptr;
			return resourceList->Get(key);
		}

		static size_t GetNumResources()
		{
			size_t count = 0;
			for (auto it = s_Instance.m_ResourceLists.begin(); it != s_Instance.m_ResourceLists.end(); ++it)
				count += it->second->GetNumResources();
			return count;
		}

	private:
		Resources(){}
		~Resources() { Flush(); }

		template <typename T>
		static ResourceList<std::string, T>* GetResourceList()
		{
			size_t hash = typeid(T).hash_code();
			auto it = s_Instance.m_ResourceLists.find(hash);

			if (it == s_Instance.m_ResourceLists.end()) return nullptr;
			return (ResourceList<std::string, T>*) s_Instance.m_ResourceLists[hash].get();
		}

		static Resources* GetInstance() { return &s_Instance; }

	private:
		static Resources s_Instance;
		std::unordered_map<size_t, Scope<ResourceListInterface>> m_ResourceLists;
	};
}