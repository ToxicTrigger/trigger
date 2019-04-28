#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "trigger_tools.h"
#include "../../cpptoml/include/cpptoml.h"

//TODO:: Add using macro in import * export component_world's code. 

// Add name in Component List
// When Component Destroy or Save timing, U Can Use this Macro!
// if registed this component in WorldsUpdateList, this _component are Saving data in file.
// if u dont want save components data? dont call macro in that variable.
// so that list can be import & export names value like {"type", "value"}
// name = value name
// value = name's value
#define SAVE_VAR(type, var_name) \
{\
auto tmp = cpptoml::make_array(); \
tmp->push_back(std::string(#type)); \
std::ostringstream ss;\
ss << var_name;\
tmp->push_back(std::string(ss.str()));\
_tmp->insert(#var_name, tmp);\
}

#define SAVE_TOML(var_name, var) \
{\
_tmp->insert(#var_name, var);\
}

#define SAVE_VAR_INIT(type, var_name, var) var_name = var; SAVE_VAR(type, var_name);

namespace trigger
{
	class component
	{
	protected:
		std::shared_ptr<cpptoml::table> _params;
		std::shared_ptr<cpptoml::table> _tmp;
		
	public:
		bool active = true;

		component()
		{
			_tmp = cpptoml::make_table();
			_params = cpptoml::make_table();
			
			SAVE_VAR(bool, active);

			_params->insert(T_CLASS, _tmp);
		}

		auto get_params()->decltype(_params) 
		{
			return _params;
		}

		virtual void update(float delta) noexcept = 0;
	};
}

#endif