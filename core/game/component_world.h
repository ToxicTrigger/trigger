#ifndef COMPONENT_WORLD_H
#define COMPONENT_WORLD_H

#include <list>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>

#include "actor.h"


namespace trigger
{
	class component_world : public trigger::component
	{
		typedef std::chrono::high_resolution_clock time;
		typedef std::chrono::time_point<std::chrono::steady_clock> Time;

	private:
		std::list<component*> components;
		Time start_time;
		std::chrono::duration<float> delta_time;
		std::chrono::duration<float> run_time;
		std::thread main_thread;
		std::mutex lock;

	public:
		float gravity = -9.8f;
		bool use_thread;
		std::string name;
	public:
		//Build a new World
		explicit inline component_world(bool UseThread)
		{
			components = std::list<component*>();
			start_time = time::now();

			use_thread = UseThread;
			if (UseThread)
			{
				main_thread = std::thread(&component_world::update, this, delta_time.count());
			}
		}

		explicit inline component_world(bool UseThread, std::string name)
		{
			components = std::list<component*>();
			start_time = time::now();
			set_name(name);

			use_thread = UseThread;
			if (UseThread)
			{
				main_thread = std::thread(&component_world::update, this, delta_time.count());
			}
		}

		inline float get_delta_time() const noexcept
		{
			return delta_time.count();
		}

		inline void set_name(const std::string name)
		{
			this->name = name;
		}
		inline const std::string& get_name()
		{
			return this->name;
		}

		template<typename T>
		inline constexpr T* get() const
		{
			for (auto i : components)
			{
				auto t = dynamic_cast<T*>(i);
				if (t != nullptr)
				{
					return t;
				}
			}
			return nullptr;
		};

		inline std::list<component*> get_all() const
		{
			return this->components;
		}

		template<typename T>
		inline constexpr std::list<T*> get_components()
		{
			std::list<T*> tmp = std::list<T*>();
			for (auto i : components)
			{
				auto t = dynamic_cast<T*>(i);
				if (t != nullptr)
				{
					tmp.push_back(t);
				}
			}
			return tmp;
		};

		inline component* get(unsigned int index) noexcept
		{
			if (index >= components.size()) return nullptr;

			auto i = components.begin();
			std::advance(i, index);
			return *i;
		}

		inline constexpr bool delete_component(component *target) noexcept
		{
			if (target != nullptr && components.size() != 0)
			{
				lock.lock();
				components.remove(target);
				lock.unlock();
				return true;
			}
			return false;
		}

		//add component in world-component-list
		inline constexpr void add(component * com) noexcept
		{
			if (com != nullptr) components.push_back(com);
		}

		inline void clean_component() noexcept
		{
			if (components.size() != 0)
			{
				auto delete_list = std::list<component*>();
				for (auto i : components)
				{
					if (!i->active) delete_list.push_back(i);
				}

				for (auto i : delete_list)
				{
					components.remove(i);
				}
			}
		}

		//simulating world
		inline void update(float delta) noexcept
		{
			while (use_thread)
			{
				if (components.size() != 0)
				{
					while (this->active)
					{
						update_all();
					}
				}
			}
		}

		void update_all()
		{
			if (components.size() != 0)
			{
				run_time = std::chrono::duration_cast<std::chrono::duration<float>>(time::now() - start_time);
				auto t = time::now();
				lock.lock();
				for (auto i : components)
				{
					if (i != nullptr)
					{
						if (i->active)
						{
							i->update(this->delta_time.count() * time_scale * i->time_scale);
						}
					}
				}
				lock.unlock();
				delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(time::now() - t);
			}
		}

		//TODO
		static bool save_world(std::string p, std::string n, component_world *w)
		{
			auto map = cpptoml::make_table();
			auto set = cpptoml::make_table();
			auto actors = cpptoml::make_table();

			std::ofstream o(p + "/" + n);
			if (!o.is_open()) return false;
			auto ac = w->get_components<actor>();

			for (auto& i : ac)
			{
				actors->insert(i->name, i->get_params());
			}

			set->insert("type", "map");
			set->insert("gravity", w->gravity);
			set->insert("use_thread", w->use_thread);
			set->insert("name", w->name);
			set->insert(T_CLASS, w->get_params());
			map->insert("setting", set);

			map->insert(w->get_name(), actors);
			o << *map;
			o.close();
			return true;
		}

		//TODO
		static inline component_world* load_world(std::string path)
		{
			auto map = cpptoml::parse_file(path);
			auto set = cpptoml::make_table();
			set = map->get_table("setting");
			auto type = set->get_as<std::string>("type").value_or("unknown");
			if (!type._Equal("map")) return nullptr;

			auto com = set->get_table("trigger::component_world");
			auto actors = cpptoml::make_table();

			auto world = new component_world(set->get_as<bool>("use_thread").value_or(true));
			world->gravity = (float)set->get_as<double>("gravity").value_or(-9.8f);
			world->name = set->get_as<std::string>("name").value_or("untitled");

			auto super = com->get_table("trigger::component");
			world->active = super->get_as<bool>("active").value_or(true);
			world->time_scale = (float)super->get_as<double>("time_scale").value_or(1.0f);

			actors = map->get_table(world->name);
			int count = 0;
			for (const auto& i : *actors)
			{
				auto t = i.second->as_table();
				auto comp = t->get_table("trigger::component");
				auto ac = new trigger::actor();
				ac->name = i.first;
				//TODO nn 
				// ���� �̸��� ���� �� 
				count++;
				world->add(ac);
			}

			return world;
		}

		~component_world()
		{
			components.clear();
			main_thread.join();
		}
	};
}

#endif