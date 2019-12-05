#ifndef COMPONENT_WORLD_H
#define COMPONENT_WORLD_H

#include <list>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>
#include <map>


#include "../tools/macros.h"
#include "transform.h"

#include "pool.h"

#include "../../../trigger-component/components.h"

using namespace trigger;

namespace trigger
{
	class world
	{
		typedef std::chrono::high_resolution_clock time;
		typedef std::chrono::time_point<std::chrono::steady_clock> Time;

	private:
		Time start_time;
		std::chrono::duration<float> delta_time;
		Time old_time;
		Time new_time;

		std::chrono::duration<float> run_time;
		std::thread main_thread;
		std::mutex lock;
		float time_scale = 1;
		int fixed_time = 20;
		bool active = true;
		glm::fmat4x4 world_space;

	public:
		bool execute = false;
		bool pause = false;
		trigger::tools::pool<transform> transforms;
		std::map<hash_id, trigger::tools::pool_obj<transform> > using_transforms;
		std::map<hash_id, transform> _cpy_using_transforms;

		float gravity = -9.8f;
		bool use_thread;
		std::string name;

	public:
		//Build a new World
		explicit inline world(bool UseThread, std::string name)
		{
			start_time = time::now();
			delta_time = std::chrono::duration<float>();
			old_time = time::now();
			set_name(name);
			fixed_time = 20;

			use_thread = UseThread;
			if (UseThread)
			{
				main_thread = std::thread(&world::update_all, this);
			}
			trigger::manager::class_manager::get_instance()->get_class_array()->clear();
			reload();
		}

		explicit inline world()
		{
			start_time = time::now();
			delta_time = std::chrono::duration<float>();
			old_time = time::now();
			set_name("World");
			fixed_time = 20;

			use_thread = true;
			if (use_thread)
			{
				main_thread = std::thread(&world::update_all, this);
			}
			trigger::manager::class_manager::get_instance()->get_class_array()->clear();
			reload();
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
			for (auto&& i : this->using_transforms)
			{
				auto t = dynamic_cast<T*>(i.second.data);
				if (t != nullptr)
				{
					return t;
				}
			}
			return nullptr;
		};

		template<typename T>
		inline std::list<T*> get_objects()
		{
			std::list<T*> tmp = std::list<T*>();
			for (auto&& i : this->using_transforms)
			{
				auto t = dynamic_cast<T*>(i.second.data);
				if (t != nullptr)
				{
					tmp.push_back(t);
				}
			}
			return tmp;
		};

		inline transform* get(hash_id index) noexcept
		{
			if (index >= using_transforms.size()) return nullptr;

			auto i = using_transforms.begin();
			std::advance(i, index);
			return i->second.data;
		}

		inline constexpr bool delete_object(transform* target)
		{
			if (target != nullptr && using_transforms.size() != 0)
			{
				using_transforms.erase(target->get_instance_id());
				//delete target;
				return true;
			}
			return false;
		}

		inline bool delete_object(hash_id id)
		{
			auto i = using_transforms.begin();
			std::advance(i, id);
			if (i->second.use)
			{
				transforms.free(std::move(i->second));
				return true;
			}
			return false;
		}

		//add component in world-component-list
		inline transform* add(transform* com) noexcept
		{
			hash_id id;
			if (com != nullptr)
			{
				auto tmp = transforms.use();
				*tmp.data = *com;

				id = tmp->get_instance_id();
				using_transforms.insert
				(
					std::pair<int, trigger::tools::pool_obj<transform>>(com->get_instance_id(), std::move(tmp))
				);
			}

			return using_transforms[id].data;
		}

		//add component in world-component-list
		inline transform* add() noexcept
		{
			hash_id id = make_hash_code();
			auto tmp = transforms.use();
			tmp->set_instance_id(id);
			
			using_transforms.insert
			(
				std::pair<int, trigger::tools::pool_obj<transform>>(tmp->get_instance_id(), std::move(tmp))
			);


			return using_transforms[id].data;
		}

		inline void clean_world() noexcept
		{
			if (using_transforms.size() != 0)
			{
				for (auto&& i : using_transforms)
				{
					if (!i.second->active) this->transforms.free(std::move(i.second));
				}
			}
		}

		inline void empty_world() noexcept
		{
			this->using_transforms.clear();
		}

		void backup()
		{
			_cpy_using_transforms.clear();
			for (auto&& i : this->using_transforms)
			{
				auto tmp = transform(*i.second.data);
				tmp.clear_components();
				//Component Copy
				for (auto&& k : *i.second->get_components())
				{
					tmp.add_component(*k.second);
				}

				tmp.set_instance_id(i.first);

				_cpy_using_transforms.insert({i.second->get_instance_id(), tmp});
			}
			this->new_time = time::now();
		}

		void restore()
		{
			for (auto&& i : this->using_transforms)
			{
				this->lock.lock();
				auto tmp = _cpy_using_transforms[i.first];
				transforms.get_raw_data()->at(i.second.index) = tmp;
				
				this->lock.unlock();
			}
		}

		void update_all()
		{
			do
			{
				if (execute)
				{
					this->lock.lock();
					std::this_thread::sleep_for(std::chrono::milliseconds(this->fixed_time));
					this->old_time = new_time;
					this->new_time = time::now();
					delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(new_time - old_time);
					run_time = std::chrono::duration_cast<std::chrono::duration<float>>(time::now() - start_time);

					// go back execute State!
					for (auto&& i : using_transforms)
					{
						if (i.second.data != nullptr && i.second->active)
						{
							i.second->update(this->delta_time.count() * time_scale * i.second->time_scale);
						}
					}

					this->lock.unlock();
				}
			} while (use_thread && this->active);
		}

		static bool save_world(std::string p, std::string n, world* w)
		{
			std::ofstream o(p + slash + n);
			if (!o.is_open()) return false;

			auto map = cpptoml::make_table();
			auto set = cpptoml::make_table();
			auto actors = cpptoml::make_table();

			auto ac = w->get_objects<transform>();

			set->insert("ObjectSize", ac.size());
			for (auto& i : ac)
			{
				actors->insert(*i->get_name(), i->save());
			}

			set->insert("type", "map");
			set->insert("gravity", w->gravity);
			set->insert("use_thread", w->use_thread);
			set->insert("name", w->name);
			map->insert("setting", set);

			map->insert(w->get_name(), actors);
			o << *map;
			o.close();
			return true;
		}

		static inline world* load_world(std::string path)
		{
			auto map = cpptoml::parse_file(path);
			auto set = cpptoml::make_table();
			set = map->get_table("setting");
			auto type = set->get_as<std::string>("type").value_or("unknown");
			if (hash_str("map") != hash_str(type.c_str())) return nullptr;

			auto scene_objs = map->get_table("SceneObjects");
			auto actors = cpptoml::make_table();

			auto world = new trigger::world(set->get_as<bool>("use_thread").value_or(true), set->get_as<std::string>("name").value_or("World"));
			world->gravity = (float)set->get_as<double>("gravity").value_or(-9.8f);
			world->name = set->get_as<std::string>("name").value_or("untitled");

			for (const auto& i : *scene_objs)
			{
				auto tmp = new trigger::transform
				(
					vec3((0.0F), (0.0F), (0.0F)),
					vec3((1.0F), (1.0F), (1.0F)),
					vec3((0.0F), (0.0F), (0.0F)),
					i.first
				);

				auto tab = scene_objs->get_table(i.first);
				for (auto ii : *tab)
				{
					auto dd = tab->get_table(ii.first);
					tmp->set_name(get_data<std::string>(dd, "name"));
					tmp->set_instance_id(get_data<int>(dd, "instance_id"));
				}
				world->add(tmp);
			}

			return world;
		}

		~world()
		{
			main_thread.join();
		}
	};
}

#endif