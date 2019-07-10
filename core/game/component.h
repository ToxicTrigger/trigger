#ifndef COMPONENT_H
#define COMPONENT_H
#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include "trigger_tools.h"
#include "../../cpptoml/include/cpptoml.h"
#include "transform.h"
#include "class_manager.h"
#include "property.h"

//TODO:: Add using macro in import * export component_world's code.

// Add name in Component List
// When Component Destroy or Save timing, U Can Use this Macro!
// if registed this component in WorldsUpdateList, this _component are Saving data in file.
// if u dont want save components data? dont call macro in that variable.
// so that list can be import & export names value like {"type", "value"}
// name = value name
// value = name's value
#define SAVE_VAR(type, var_name)               \
	{                                          \
		auto tmp = cpptoml::make_array();      \
		tmp->push_back(std::string(#type));    \
		std::ostringstream ss;                 \
		ss << var_name;                        \
		tmp->push_back(std::string(ss.str())); \
		_tmp->insert(#var_name, tmp);          \
	}

#define SAVE_STR(name, var)               \
	{                                     \
		auto tmp = cpptoml::make_array(); \
		tmp->push_back(var);              \
		_tmp->insert(#name, tmp);         \
	}

#define SAVE_TOML(var_name, var)      \
	{                                 \
		_tmp->insert(#var_name, var); \
	}

#define SAVE_VAR_INIT(type, var_name, var) \
	var_name = var;                        \
	SAVE_VAR(type, var_name);

static hash_id make_hash_code_()
{
	auto t = std::chrono::system_clock::now();
	auto hash = static_cast<int>(t.time_since_epoch().count());
	std::srand(hash);
	hash *= std::rand() >= RAND_MAX / 2 ? 1 : -1;
	return hash;
}

template <typename T>
inline T get_data(std::shared_ptr<cpptoml::table> array, const std::string &key)
{
	auto d = array->get_array_of<std::string>(key);
	T a;
	for (const auto &v : *d)
	{
		std::stringstream s(v);
		if ((s >> a).fail())
			continue;
	}
	return a;
}

#define REGI_CLASS(type)         \
	_params->insert(type, _tmp); \
	trigger::manager::class_manager::get_instance()->get_class_array()->insert(std::pair<std::string, trigger::component *>(type, dynamic_cast<trigger::component*>(this)));


namespace trigger
{
	class transform;
	class component;
	class property;

	class component
	{
	protected:
		std::shared_ptr<cpptoml::table> _params;
		std::shared_ptr<cpptoml::table> _tmp;
		std::shared_ptr<cpptoml::table> toml_properties;
		hash_id type_code;
		std::string type_name;
		hash_id instance_id;

	public:
		std::map< hash_id, property> properties;
		bool active = true;

		component()
		{
			this->instance_id = make_hash_code_();
			_tmp = cpptoml::make_table();
			_params = cpptoml::make_table();
			toml_properties = cpptoml::make_table();
			type_code = hash_str("trigger::component");
			type_name = "trigger::component";
			REGI_CLASS("trigger::component");
			active = true;
			this->properties = std::map<hash_id, property>();
		}

		component(std::string type)
		{
			this->instance_id = make_hash_code_();
			_tmp = cpptoml::make_table();
			_params = cpptoml::make_table();
			toml_properties = cpptoml::make_table();
			type_code = hash_str(type.c_str());
			type_name = type;
			this->properties = std::map<hash_id, property>();
			active = true;
			property(active, property::data_type::Bool, "active", &this->properties);
			property(type_code, property::data_type::HashID, "type_code", &this->properties);
			property(instance_id, property::data_type::HashID, "instance_id", &this->properties);
			REGI_CLASS(type);
		}

		template<typename T = trigger::component>
		static void regi_component()
		{
			T* com = new T();
			trigger::manager::class_manager::get_instance()->
				get_class_array()->
				insert(
					std::pair<std::string, trigger::component *>(T_CLASS, dynamic_cast<trigger::component*>(com)));
		}
		
		//save 함수는 자신이 들고있는 toml 테이블을 갱신시키는 함수임
		//왜 그렇게 되냐면 Unknown 타입에 대한 대응을 위해서임
		// unknown 타입을 스트링으로 바꾸기 위해서는 자신의 타입을 알아야 하는데 이를 
		// add_unknown( ) 함수를 통해 테이블에 새로 추가하는 개념이기 때문임 
		// 아직 미구현이니 일단 이대로 진행함
		auto save() -> decltype(auto)
		{
			auto proper = cpptoml::make_table();
			auto pro = cpptoml::make_table();

			for (auto& i : this->properties)
			{
				std::ostringstream ss;
				switch (i.second.type)
				{
				case property::data_type::Int:
					ss << i.second.get_int().value_or(-999);
					break;
				case property::data_type::Float:
					ss << i.second.get_float().value_or(-0.999f);
					break;
				case property::data_type::Double:
					ss << i.second.get_double().value_or(-0.999);
					break;
				case property::data_type::String:
					ss << i.second.get_string().value_or("FAILED");
					break;
				case property::data_type::Bool:
					ss << i.second.get<bool>();
					break;
				case property::data_type::SizeT:
					ss << i.second.get<size_t>();
					break;
				case property::data_type::HashID:
					ss << i.second.get<hash_id>();
					break;
				case property::data_type::Unknown:
					break;
				}
				pro->insert(i.second.get_name(), std::string(ss.str()));
			}
			proper->insert("Properties", pro);
			return proper;
		};

		std::map< hash_id, property> load(std::shared_ptr<cpptoml::table> table)
		{
			auto map = std::map< hash_id, property>();
			for (auto& i : *table)
			{
			}

			return map;
		}


		virtual ~component()
		{
			save();
		}

		size_t get_type_id()
		{
			return this->type_code;
		}

		std::string get_type_name()
		{
			return this->type_name;
		}

		auto get_params() -> decltype(_params)
		{
			return _params;
		}

		virtual void update(float delta) noexcept {};
	};
} // namespace trigger

#endif