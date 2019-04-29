#ifndef COMPONENT_WORLD_H
#define COMPONENT_WORLD_H

#include <list>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>
#include <map>
#include <Windows.h>

#include "transform.h"
using namespace trigger;

namespace trigger
{
	class world
	{
		typedef std::chrono::high_resolution_clock time;
		typedef std::chrono::time_point<std::chrono::steady_clock> Time;

	private:
		std::map<int, transform*> objects;
		Time start_time;
		std::chrono::duration<float> delta_time;
		Time old_time;

		std::chrono::duration<float> run_time;
		std::thread main_thread;
		std::mutex lock;
		float time_scale = 1;
		bool active = true;

	public:
		float gravity = -9.8f;
		bool use_thread;
		std::string name;

	public:
		//Build a new World
		explicit inline world(bool UseThread)
		{
			start_time = time::now();
			delta_time = std::chrono::duration<float>();
			old_time = time::now();
			objects = std::map<int, transform*>();

			use_thread = UseThread;
			if (UseThread)
			{
				main_thread = std::thread(&world::update_all, this);
			}
		}

		explicit inline world(bool UseThread, std::string name)
		{
			objects = std::map<int, transform*>();
			start_time = time::now();
			delta_time = std::chrono::duration<float>();
			old_time = time::now();
			set_name(name);

			use_thread = UseThread;
			if (UseThread)
			{
				main_thread = std::thread(&world::update_all, this);
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
		inline T* get() const
		{
			for (auto i : objects)
			{
				auto t = dynamic_cast<T*>(i);
				if (t != nullptr)
				{
					return t;
				}
			}
			return nullptr;
		};

		inline std::map<int, transform*> get_all() const
		{
			return this->objects;
		}

		template<typename T>
		inline std::list<T*> get_objects()
		{
			std::list<T*> tmp = std::list<T*>();
			for (auto i : objects)
			{
				auto t = dynamic_cast<T*>(i.second);
				if (t != nullptr)
				{
					tmp.push_back(t);
				}
			}
			return tmp;
		};

		inline transform* get(unsigned int index) noexcept
		{
			if (index >= objects.size()) return nullptr;

			auto i = objects.begin();
			std::advance(i, index);
			return i->second;
		}

		inline constexpr bool delete_object(transform *target) noexcept
		{
			if (target != nullptr && objects.size() != 0)
			{
				lock.lock();
				objects.erase(target->get_instance_id());
				lock.unlock();
				return true;
			}
			return false;
		}

		//add component in world-component-list
		inline constexpr void add(transform * com) noexcept
		{
			if (com != nullptr)
				objects.insert
				(
					std::pair<int, transform*>(com->get_instance_id(), com)
				);
		}

		inline void clean_world() noexcept
		{
			if (objects.size() != 0)
			{
				auto delete_list = std::list<transform*>();
				for (auto i : objects)
				{
					if (!i.second->active) delete_list.push_back(i.second);
				}

				for (auto i : delete_list)
				{
					objects.erase(i->get_instance_id());
				}
			}
		}

		void update_all()
		{
			do
			{
				this->lock.lock();
				this->old_time = time::now();
				
				if (objects.size() != 0)
				{
					run_time = std::chrono::duration_cast<std::chrono::duration<float>>(time::now() - start_time);
					for (auto i : objects)
					{
						if (i.second != nullptr)
						{
							if (i.second->active)
							{
								i.second->update(this->delta_time.count() * time_scale * i.second->time_scale);
							}
						}
					}
				}
				delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(time::now() - old_time);
				this->lock.unlock();
			} while (use_thread && this->active);
		}

		////TODO
		//static bool save_world(std::string p, std::string n, component_world *w)
		//{
		//	auto map = cpptoml::make_table();
		//	auto set = cpptoml::make_table();
		//	auto actors = cpptoml::make_table();

		//	std::ofstream o(p + "/" + n);
		//	if (!o.is_open()) return false;
		//	auto ac = w->get_components<transform>();

		//	for (auto& i : ac)
		//	{
		//		actors->insert(i->name, i->get_params());
		//	}

		//	set->insert("type", "map");
		//	set->insert("gravity", w->gravity);
		//	set->insert("use_thread", w->use_thread);
		//	set->insert("name", w->name);
		//	map->insert("setting", set);

		//	map->insert(w->get_name(), actors);
		//	o << *map;
		//	o.close();
		//	return true;
		//}

		////TODO
		//static inline component_world* load_world(std::string path)
		//{
		//	auto map = cpptoml::parse_file(path);
		//	auto set = cpptoml::make_table();
		//	set = map->get_table("setting");
		//	auto type = set->get_as<std::string>("type").value_or("unknown");
		//	//if (!type._Equal("map")) return nullptr;

		//	auto com = set->get_table("trigger::component_world");
		//	auto actors = cpptoml::make_table();

		//	auto world = new component_world(set->get_as<bool>("use_thread").value_or(true));
		//	world->gravity = (float)set->get_as<double>("gravity").value_or(-9.8f);
		//	world->name = set->get_as<std::string>("name").value_or("untitled");

		//	auto super = com->get_table("trigger::component");
		//	world->active = super->get_as<bool>("active").value_or(true);
		//	world->time_scale = (float)super->get_as<double>("time_scale").value_or(1.0f);

		//	actors = map->get_table(world->name);
		//	int count = 0;
		//	for (const auto& i : *actors)
		//	{
		//		auto t = i.second->as_table();
		//		auto comp = t->get_table("trigger::component");
		//		auto ac = new trigger::actor();
		//		ac->name = i.first;
		//		//TODO
		//		count++;
		//		world->add(ac);
		//	}

		//	return world;
		//}

		~world()
		{
			objects.clear();
			main_thread.join();
		}
	};
}

#endif