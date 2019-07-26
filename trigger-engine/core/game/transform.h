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
		std::vector<trigger::component*> components;
		std::string name;

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
			this->instance_id = make_hash_code();
			std::string n(name);
			if (name.find("Object") != std::string::npos)
			{
				n += ":";
				n += std::to_string(instance_id);
			}
			this->name = n;
			this->components = std::vector<trigger::component*>();
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

		void set_instance_id(int code)
		{
			this->instance_id = code;
			save();
		}


		virtual ~transform();

		virtual void update(float delta) noexcept;

		template<typename T>
		T* get_component()
		{
			for (auto i : this->components)
			{
				auto t = dynamic_cast<T*>(i);
				if(t != nullptr)
				{
					return t;
				}
			}
			return nullptr;
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
			this->components.push_back(com);
			save();

			return true;
		}

		template<typename T>
		bool add_component(T* component)
		{

			this->components.push_back(component);
			save();

			return true;
		}

		template<typename T>
		bool add_component(T component)
		{

			this->components.push_back(new T(component));
			save();

			return true;
		}

		const int get_instance_id() const;
		std::vector<trigger::component*> get_components() const;
	};
};