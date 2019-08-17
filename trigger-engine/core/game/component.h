#ifndef COMPONENT_H
#define COMPONENT_H
#define GLM_ENABLE_EXPERIMENTAL 1

#include <string>
#include <iostream>
#include <chrono>
#include "trigger_tools.h"
#include "../../cpptoml/include/cpptoml.h"
#include "property.h"
#include <optional>
#include "../impl/impl_singleton.h"

template <typename T>
T *trigger::impl::impl_singletone<T>::_inst = 0;

#define GET_CLASS(type) trigger::manager::class_manager::get_instance()->get_class<type>(#type)
#define get_my_transform() ((trigger::transform*)transform_ptr)

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

namespace trigger
{
class property;
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

	auto *get_class_array()
	{
		return &this->CLASS_ARRAY;
	}

	template <typename T>
	static void regi_component(std::string type)
	{
		T *com = new T();
		trigger::manager::class_manager::get_instance()->get_class_array()->insert(
			std::pair<std::string, trigger::component *>(
				             type, dynamic_cast<trigger::component *>(com)
				)
		);
	}

	template <typename T>
	auto clone(std::string name,T author)
	{
		auto com = this->CLASS_ARRAY.at(name);
		com->transform_ptr = (void*)author;
		com->set_instance_id(make_hash_code());
		return com->clone();
	}

	template <typename T>
	void clone_and_add(std::string name, T author)
	{
		auto com = this->CLASS_ARRAY.at(name);
		com->transform_ptr = (void*)author;
		author->add_component(com->clone());
	}

	template <typename C>
	std::optional<C> get_class(std::string type)
	{
		return dynamic_cast<C>(this->CLASS_ARRAY[type]);
	}
};
} // namespace manager

class component
{
protected:
	std::shared_ptr<cpptoml::table> _params;
	std::shared_ptr<cpptoml::table> _tmp;
	std::shared_ptr<cpptoml::table> toml_properties;
	hash_id type_code;
	std::string type_name;
	hash_id instance_id;
	hash_id _transform;


public:
	std::map<hash_id, property> properties;
	bool active = true;
	void* transform_ptr;

	component()
	{
		this->instance_id = make_hash_code_();
		_tmp = cpptoml::make_table();
		_params = cpptoml::make_table();
		toml_properties = cpptoml::make_table();
		type_code = hash_str("trigger::component");
		type_name = "trigger::component";
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
		property(type_code, property::data_type::HashID, false, "type_code", &this->properties);
		property(instance_id, property::data_type::HashID, false,"instance_id", &this->properties);
	}

	template <typename T>
	bool set_property(std::string name, T val)
	{
		auto key = hash_str(name.c_str());
		if (this->properties.find(key) != this->properties.end())
		{
			this->properties[key].set<T>(val);
			return true;
		}
		return false;
	}

	auto save() -> decltype(auto)
	{
		auto proper = cpptoml::make_table();
		auto pro = cpptoml::make_table();

		for (auto i : this->properties)
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

	//TODO
	std::map<hash_id, property> load(std::shared_ptr<cpptoml::table> table)
	{
		auto map = std::map<hash_id, property>();
		for (auto &i : *table)
		{
		}

		return map;
	}

	virtual ~component()
	{
		this->properties.clear();
		this->transform_ptr = nullptr;
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

	const hash_id get_instance_id() const
	{
		return this->instance_id;
	}

	void set_instance_id(hash_id code)
	{
		this->instance_id = code;
		save();
	}

	virtual void update(float delta) {};
	virtual void draw_editor() {};
	virtual component* clone() const { return new component(*this); };
};
} // namespace trigger

#endif