#pragma once

#include "UIElementInterface.h"

namespace Kinematics {
	namespace UI {
		template<typename ... Types>
		class Inflator
		{
		public:
			Inflator()
				: m_Active(false)
			{

			}

			Ref<UIElementInterface> Inflate(Types ... data)
			{
				if(m_Active)
					return m_Inflator(data...);
				return Ref<UIElementInterface>();
			}

			void SetInflator(FunctionCallback<Ref<UIElementInterface>, Types...> inflator) 
			{ 
				m_Inflator = inflator; 
				m_Active = true;
			}

		private:
			FunctionCallback<Ref<UIElementInterface>, Types...> m_Inflator;
			bool m_Active;
		};
	}
}