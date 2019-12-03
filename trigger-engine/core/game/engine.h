#ifndef ENGINE_H
#define ENGINE_H

#include "../../renderer/renderer.h"
#include "world.h"
#include "mini_core.h"
#include "../core/editor/main_editor.h"

#ifdef _WIN64
#include <windows.h>
#endif

//MOVE pool.h
namespace trigger
{
	namespace tools
	{
		template <class T>
		class pool;

		template <class T>
		class pool_obj
		{
		public:
			bool use;
			T* data;
			pool<T>* p_ptr;
			size_t index;

			T* operator*() = delete;

			T* operator->()
			{
				return data;
			}

			pool_obj<T>& operator=(pool_obj<T> val)
			{
				index = val.index;
				this->data = val.data;
				this->p_ptr = val.p_ptr;
				this->use = val.use;

				val.p_ptr = nullptr;
				val.data = nullptr;
				val.use = true;
				val.index = 0;
				return *this;
			}

			pool_obj()
			{
				index = 0;
				this->data = nullptr;
				this->p_ptr = nullptr;
				this->use = false;
			}

			pool_obj(pool_obj<T>&& val)
			{
				index = val.index;
				this->data = val.data;
				this->p_ptr = val.p_ptr;
				this->use = val.use;

				val.p_ptr = nullptr;
				val.data = nullptr;
				val.use = true;
				val.index = 0;
			}

			~pool_obj()
			{
				if (this->p_ptr != nullptr && this->data != nullptr)
				{
					this->p_ptr->free(std::move(*this));
				}
			}
		};

		template <class T>
		class pool
		{
		public:
			template <class ... _Tval>
			pool(size_t capacity, _Tval&&... inits)
			{
				this->size = 0;
				objects = std::vector< pool_obj<T> >(capacity);
				this->capacity = objects.capacity();
				objs_data = std::vector< T >(capacity, std::forward<_Tval>(inits)...);
				init = T(std::forward<_Tval>(inits)...);
				for (int i = 0; i < capacity; ++i)
				{
					objects[i].index = i;
					objects[i].use = false;
					objects[i].p_ptr = nullptr;
					objects[i].data = &objs_data[i];
				}
			}

			pool_obj<T> use()
			{
				if (this->size < this->capacity)
				{
					for (size_t i = 0; i < capacity; ++i)
					{
						if (!objects[i].use)
						{
							this->size += 1;
							objects[i].use = true;
							objects[i].p_ptr = this;
							objects[i].data = &objs_data[i];
							objects[i].index = i;
							return std::move(objects[i]);
						}
					}
				}

				return {};
			}

			bool free(pool_obj<T>&& lvalue)
			{
				if (lvalue.p_ptr == this)
				{
					size_t i = lvalue.index;
					this->size -= 1;
					objects[i].use = false;
					objects[i].index = i;
					objects[i].p_ptr = nullptr;
					objects[i].data = &objs_data[i];
					objs_data[i] = init;

					lvalue.data->~T();

					lvalue.p_ptr = nullptr;
					lvalue.data = nullptr;
					lvalue.use = false;
					lvalue.index = i;
					return true;
				}
				return false;
			}


			size_t get_using_count() const
			{
				return this->size;
			}

			size_t get_capacity() const
			{
				return this->capacity;
			}



		private:
			std::vector< pool_obj<T> > objects;
			std::vector< T > objs_data;
			T init;
			size_t capacity;
			size_t size;
		};

	}
}

namespace trigger
{
	namespace rend
	{
		class renderer;
	}

	namespace core
	{
		enum engine_state
		{
			not_inited,
			initing,
			inited,

			awake,
			start,
			run,
			paused,

			stop,
			clean_up,
			shut_down
		};

		class engine
		{
		protected:
			trigger::core::engine_state state;

		public:
			trigger::rend::renderer *renderer;
			trigger::world *editors;
			trigger::world *object;
			trigger::edit::main_editor *main_editor;

		public:
#ifdef _WIN64
			engine(int w, int h, bool edit_mod)
			{
				this->init(w, h, edit_mod);
				this->state = engine_state::inited;
				this->run();
			}
			bool init(int w, int h, bool edit_mod);
#else
			engine(int w, int h, bool edit_mod)
			{
				this->init(w, h, edit_mod);
			}
			bool init(int w, int h, bool edit_mod);
#endif
			int run();
			engine_state get_state() const
			{
				return this->state;
			}

			~engine();
		};
	} // namespace core
} // namespace trigger

#endif