#pragma once

#include "component.h"

typedef glm::fvec2 vec2;
typedef glm::fvec3 vec3;
typedef glm::fvec4 vec4;

static int make_hash_code()
{
	auto t = std::chrono::system_clock::now();
	auto hash = static_cast<int>(t.time_since_epoch().count());
	std::srand(hash);
	hash *= std::rand() >= RAND_MAX/2 ? 1 : -1;
	return hash;
}

namespace trigger
{
	class component;
	class transform;

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
		

		transform(vec3 pos = vec3(0.0f, 0.0f, 0.0f), vec3 scale = vec3(1.0f, 1.0f, 1.0f), vec3 rot = vec3(0.0f, 0.0f, 0.0f), std::string name = "Object") : trigger::component(T_CLASS)
		{
			this->real_position = pos;
			this->real_scale = scale;
			this->real_rotation = rot;
			this->name = name;
			this->components = std::vector<trigger::component*>();
			this->instance_id = make_hash_code();
			save();
		};

		std::shared_ptr<cpptoml::table> get_component_toml(std::vector<trigger::component*> vec)
		{
			auto table = cpptoml::make_table();
			for(auto&& i : vec)
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

		std::string get_name()
		{
			return this->name;
		}

		void set_instance_id(int code)
		{
			this->instance_id = code;
			save();
		}

		virtual void save()
		{
			SAVE_TOML(components, get_component_toml(components));
			SAVE_STR(name, name);
			SAVE_STR(real_position , glm::to_string(real_position));
			SAVE_VAR(int, instance_id);
			SAVE_VAR(size_t, type_code);
			SAVE_STR(data, cast<char*>(this));
		}

		virtual ~transform();

		void update(float delta) noexcept;

		template<typename T>
		T* get_component()
		{
			for (auto i : this->components)
			{
				auto t = dynamic_cast<T*>(i);
				if (t != nullptr)
				{
					return t;
				}
			}
			return nullptr;
		};

		template<typename T>
		bool add_component()
		{
			if (this->get_component<T>() == nullptr)
			{
				T* com = new T();
				this->components.push_back(com);
				save();
				return true;
			}
			return false;
		}

		template<typename T>
		bool add_component(T* component)
		{
			if (this->get_component<T>() == nullptr)
			{
				this->components.push_back(component);
				save();
				return true;
			}
			return false;
		}

		const int get_instance_id() const;
		auto get_components() const;
	};
};