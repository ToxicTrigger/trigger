#pragma once
#include "component.h"
#include <optional>
#include "../impl/impl_singleton.h"
template <typename T> T * trigger::impl::impl_singletone<T>::_inst = 0;

#define GET_CLASS(type) trigger::manager::class_manager::get_instance()->get_class<type>(#type)

namespace trigger
{
	class component;
	namespace manager
	{
		class class_manager : public trigger::impl::impl_singletone<class_manager>
		{
		private:
			std::map<std::string, trigger::component *> CLASS_ARRAY;

		public:
			class_manager()
			{
				CLASS_ARRAY = std::map<std::string, trigger::component *>();
			}

			auto* get_class_array()
			{
				return &this->CLASS_ARRAY;
			}
			
			template <typename C>
			std::optional<C*> get_class(std::string type)
			{
				if(this->CLASS_ARRAY.find(type) == this->CLASS_ARRAY.end())
				{
					trigger::component::regi_component<C>();
				}

				return dynamic_cast<C*>(this->CLASS_ARRAY[type]);
			}
		};
	} // namespace manager
} // namespace trigger
