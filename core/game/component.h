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

static int make_hash_code_()
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
	trigger::manager::class_manager::get_instance()->get_class_array().insert(std::pair<std::string, trigger::component *>(type, (trigger::component *)this));


namespace trigger
{
class transform;
class component;

class component
{
protected:
	std::shared_ptr<cpptoml::table> _params;
	std::shared_ptr<cpptoml::table> _tmp;
	size_t type_code;
	std::string type_name;
	int instance_id;
	trigger::property<int*> me;

public:
	bool active = true;

	component()
	{
		this->instance_id = make_hash_code_();
		_tmp = cpptoml::make_table();
		_params = cpptoml::make_table();

		type_code = hash_str("trigger::component");
		type_name = "trigger::component";
		SAVE_VAR(bool, active);
		SAVE_VAR(size_t, type_code);
		SAVE_VAR(int, instance_id);
		REGI_CLASS("trigger::component");
	}

	component(std::string type)
	{
		this->instance_id = make_hash_code_();
		_tmp = cpptoml::make_table();
		_params = cpptoml::make_table();
		
		me = trigger::property<int*>(new int(10), "me");
		auto a = this->me.to_string();
		auto v = *this->me.parse(a.c_str());

		auto bt = trigger::property<char*>("Heelo", "bt");
		auto test = bt.get_id();
		auto tex = bt.to_string();
		auto val = bt.parse(tex);

		SAVE_VAR(bool, active);
		SAVE_VAR(size_t, type_code);
		SAVE_VAR(int, instance_id);
		REGI_CLASS(type);
		type_code = hash_str(type.c_str());
		type_name = type;
	}

	virtual void save(){

	};

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

	virtual void update(float delta) noexcept = 0;
};
} // namespace trigger

#endif