#pragma once
#include "component.h"
#include <chrono>
#include <glm/glm.hpp>
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
	/// component head
	class transform
	{
	private:
		int hash_code;

	protected:
		vec3 real_position;
		vec3 real_scale;
		vec3 real_rotation;
		std::vector<trigger::component*> components;

	public:
		vec3 position;
		vec3 scale;
		vec3 rotation;
		bool active;
		float time_scale = 1.0f;
		std::string name;

		transform(vec3 pos = vec3(0.0f, 0.0f, 0.0f), vec3 scale = vec3(1.0f, 1.0f, 1.0f), vec3 rot = vec3(0.0f, 0.0f, 0.0f), std::string name = "Object");
		virtual ~transform();

		void update(float delta);

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
				return true;
			}
			return false;
		}

		template<typename T>
		bool add_component(T* component)
		{
			if (this->get_component<T>() == nullptr)
			{
				T* com = new T();
				auto size = sizeof(T);
				memcpy_s(com, sizeof(T), component, sizeof(T));
				this->components.push_back(com);
				return true;
			}
			return false;
		}

		const int get_instance_id() const;
		auto get_components() const;
	};
};