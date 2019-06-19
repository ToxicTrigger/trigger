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

#define SAVE_STR(name, var) \
{\
auto tmp = cpptoml::make_array(); \
tmp->push_back(var);\
_tmp->insert(#name, tmp);\
}

#define SAVE_TOML(var_name, var) \
{\
_tmp->insert(#var_name, var);\
}

#define SAVE_VAR_INIT(type, var_name, var) var_name = var; SAVE_VAR(type, var_name);

static int make_hash_code_()
{
	auto t = std::chrono::system_clock::now();
	auto hash = static_cast<int>(t.time_since_epoch().count());
	std::srand(hash);
	hash *= std::rand() >= RAND_MAX/2 ? 1 : -1;
	return hash;
}

#define SAVE(val) reinterpret_cast<char*>(&val)
#define SAVE_THIS() reinterpret_cast<char*>(this)
#define LOAD(type, val) reinterpret_cast<type##*>(val);

template <class TO, class FROM>
TO cast(FROM v)
{
	return static_cast<TO>(static_cast<void*>(v));
}

template <typename T>
inline T get_data(std::shared_ptr<cpptoml::table> array, const std::string& key)
{
	auto d = array->get_array_of<T>(key);
	T a;
	for(const auto& v : *d)
	{
		a = v;
	}
	return a;
}


inline int get_data_int(std::shared_ptr<cpptoml::table> array, const std::string& key)
{
	auto d = array->get_array_of<std::string>(key);
	int a = 0;
	for(const auto v : *d)
	{
		std::stringstream s(v);
		if((s>>a).fail()) continue;
	}
	return a;
}


#define PRI_A 54059 /* a prime */
#define PRI_B 76963 /* another prime */
#define PRI_C 86969 /* yet another prime */
#define PRI_FIRSTH 37 /* also prime */
static unsigned hash_str(const char* s)
{
   unsigned h = PRI_FIRSTH;
   while (*s) {
     h = (h * PRI_A) ^ (s[0] * PRI_B);
     s++;
   }
   return h % PRI_C ; // or return h % C;
}

namespace trigger
{
	class transform;
	class component;
	static std::map<std::string, trigger::transform*> CLASS_ARRAY = std::map<std::string, trigger::transform*>();

	#define REGI_CLASS(type) _params->insert(type, _tmp); \
	trigger::CLASS_ARRAY.insert(std::pair<std::string, trigger::transform*>(type, (trigger::transform*)this));

	class component
	{
	protected:
		std::shared_ptr<cpptoml::table> _params;
		std::shared_ptr<cpptoml::table> _tmp;
		size_t type_code;
		std::string type_name;
		int instance_id;
		
	public:
		bool active = true;

		component()
		{
			_tmp = cpptoml::make_table();
			_params = cpptoml::make_table();
			type_code = hash_str("trigger::component");
			type_name = "trigger::component";
			this->instance_id = make_hash_code_();
			SAVE_VAR(bool, active);
			SAVE_VAR(size_t, type_code);
			SAVE_VAR(int, instance_id);
			REGI_CLASS("trigger::component");
		}
		
		component( std::string type )
		{
			this->instance_id = make_hash_code_();
			_tmp = cpptoml::make_table();
			_params = cpptoml::make_table();
			REGI_CLASS(type);
			type_code = hash_str(type.c_str());
			type_name = type;
			SAVE_VAR(bool, active);
			SAVE_VAR(size_t, type_code);
			SAVE_VAR(int, instance_id);
		}

		virtual void save(){};

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

		auto get_params()->decltype(_params) 
		{
			return _params;
		}

		virtual void update(float delta) noexcept = 0;
	};
}

#endif