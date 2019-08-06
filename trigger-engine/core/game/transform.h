#pragma once

#include "component.h"
#include <chrono>
#include <vector>
#include "../../lib/vk/glm/glm.hpp"
typedef glm::fvec2 vec2;
typedef glm::fvec3 vec3;
typedef glm::fvec4 vec4;


static int make_hash_code()
{
	auto t = std::chrono::system_clock::now();
	auto hash = static_cast<int>(t.time_since_epoch().count());
	std::srand(hash);
	hash += std::rand() % 999;
	hash *= std::rand() >= RAND_MAX / 2 ? 1 : -1;
	return hash;
}

namespace trigger
{
	class transform;
	class component;
	/// component head
	class transform : public trigger::component
	{
	private:
		int instance_id;

	protected:
		vec3 real_position;
		vec3 real_scale;
		vec3 real_rotation;
		std::map<hash_id, trigger::component*> components;
		std::string name;
		glm::fmat4x4 space;

	public:
		vec3 position;
		vec3 scale;
		vec3 rotation;
		bool active;
		float time_scale = 1.0f;
		transform* parent;
		std::vector<transform*> childs;

		transform(vec3 pos = vec3(0.0f, 0.0f, 0.0f), vec3 scale = vec3(1.0f, 1.0f, 1.0f), vec3 rot = vec3(0.0f, 0.0f, 0.0f), std::string name = "Object") : trigger::component(T_CLASS)
		{
			this->real_position = pos;
			this->real_scale = scale;
			this->real_rotation = rot;
			position = pos;
			this->scale = scale;
			this->rotation = rot;
			this->instance_id = make_hash_code();
			std::string n(name);
			if (name.find("Object") != std::string::npos)
			{
				n += ":";
				n += std::to_string(instance_id);
			}
			this->name = n;
			this->components = std::map<hash_id, trigger::component*>();
			parent = nullptr;
			childs = std::vector<transform*>();
		};

		std::shared_ptr<cpptoml::table> get_component_toml(std::vector<trigger::component*> vec)
		{
			auto table = cpptoml::make_table();
			for (auto&& i : vec)
			{
				table->insert(i->get_type_name(), i->get_params());
			}
			return table;
		}

		std::string get_string_from_table(std::shared_ptr<cpptoml::table> table)
		{
			std::ostringstream o;
			o << *table;
			return o.str();
		}

		void set_name(std::string name)
		{
			this->name = name;
			save();
		}

		void set_name(char* const name)
		{
			this->name = name;
			save();
		}

		std::string* get_name()
		{
			return &this->name;
		}


		template<typename T>
		T* get_component()
		{
			for (auto i : this->components)
			{
				auto t = dynamic_cast<T*>(i.second);
				if(t != nullptr)
				{
					return t;
				}
			}
			return nullptr;
		};

		template<typename T>
		T* get_component(hash_id id)
		{
			auto t = dynamic_cast<T*>(this->components[id]);
			return t;
		};

		template<typename T>
		bool is_it()
		{
			if (get_component<T>() == nullptr) return false;
			return true;
		}

		template<typename T>
		bool add_component()
		{
			T* com = new T();
			com->transform_ptr = (void*)this;
			this->components.insert(
				std::pair<hash_id, trigger::component*>(com->get_instance_id(), com)
			);
			save();
			return true;
		}

		template<typename T>
		bool add_component(T* component)
		{
			component->transform_ptr = (void*)this;
			this->components.insert(
				std::pair<hash_id, trigger::component*>(component->get_instance_id(), component)
			);
			save();
			return true;
		}

		template<typename T>
		bool add_component(T component)
		{
			T* com = new decltype(component)();
			com->transform_ptr = (void*)this;
			this->components.insert(
				std::pair<hash_id, trigger::component*>(com->get_instance_id(), com)
			);
			save();
			return true;
		}

		transform* get_parent();
		void set_parent(transform* target);
		std::vector<transform*> get_childs();
		transform* get_child_at(size_t index);
		void add_child(transform* child);
		void remove_parent();
		void remove_child(transform* child);
		void remove_child(size_t index);
		void clear_child();
		void clear_and_destroy_child();
	
		virtual ~transform();
		virtual void update(float delta);
		virtual transform* clone() const
		{
			return new transform(*this);
		}

		std::map<hash_id, trigger::component*> get_components() const
		{
			return this->components;
		}
	};
};