#pragma once
#include <vector>

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
			pool()
			{
				this->size = 0;
				objects = std::vector< pool_obj<T> >(10000);
				this->capacity = objects.capacity();
				init = T();

				objs_data = std::vector< T >(capacity, init);
				for (int i = 0; i < capacity; ++i)
				{
					objects[i].index = i;
					objects[i].use = false;
					objects[i].p_ptr = nullptr;
					objects[i].data = &objs_data[i];
				}
			}

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