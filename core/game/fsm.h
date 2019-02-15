#pragma once
#include <thread>
#include <string>
#include <list>
#include "component.h"
#include <iostream>
#include <memory>

namespace trigger
{
	namespace fsm
	{
		class state
		{
			const std::string name;
		public:
			explicit inline state(const std::string name = "Unknown") noexcept : name(name)
			{

			}

			inline const std::string& get_name() const noexcept
			{
				return this->name;
			}
			virtual void begin_state()
			{};
			virtual void end_state()
			{};
			virtual void update(const float delta)
			{};
		};

		class link
		{
		private:
			state *cur;
			state *next;
			int ops;

		public:
			explicit inline link() noexcept
			{
				cur = nullptr;
				next = nullptr;
				ops = 1;
			}

			explicit inline link(const state *current, const state *next) : link()
			{
				this->cur = const_cast<state*>(current);
				//this->cur._Myptr() = const_cast<state*>(current);
				this->next = const_cast<state*>(next);
			}

			inline const state* get_current_state() const noexcept
			{
				return this->cur;
			};
			inline const state* get_next_state() const noexcept
			{
				return this->next;
			};
			inline constexpr const int& get_ops() const noexcept
			{
				return this->ops;
			};
			inline constexpr void set_ops(int op) noexcept
			{
				ops = op;
			};
		};

		class map : public component
		{
		private:
			state *now_state;
			std::list<state*> states;
			std::list<link*> links;
			std::string cur_name, now_name;

			inline void simulate(float delta) noexcept
			{
				now_state->update(delta);
				for(auto i : this->links)
				{
					cur_name = i->get_current_state()->get_name();
					if(cur_name == now_name)
					{
						// ops == 0 , move now_state between link
						if(i->get_ops() == 0)
						{
							i->set_ops(i->get_ops() + 1);
							now_state->end_state();
							now_state = const_cast<state*>(i->get_next_state());
							now_state->begin_state();
							now_name = now_state->get_name();
							return;
						}
					}
				}
			}

		public:
			inline explicit map()
			{
				states = std::list<state*>();
				links = std::list<link*>();

				auto idle = new state("idle");
				add_state(idle);
				now_state = new state(idle->get_name());
				now_name = now_state->get_name();
			}

			inline explicit map(state *def_state) : map()
			{
				// inited state idle
				add_state(def_state);
				link *def = new link(now_state, def_state);
				def->set_ops(0);
				links.push_back(def);
				now_name = now_state->get_name();
			}

			inline const state* const get_state(const std::string name) const noexcept
			{
				for(auto i : states)
				{
					if(i->get_name() == name)
					{
						return i;
					}
				}
				return nullptr;
			}

			inline const state* const get_state(const state *state) const noexcept
			{
				for(auto i : states)
				{
					if(i == state) return i;
				}
				return nullptr;
			}

			inline bool link_state(const std::string state1, const std::string state2) noexcept
			{
				const state *a = get_state(state1);
				if(a != nullptr)
				{
					const state *b = get_state(state2);
					if(b != nullptr)
					{
						link *tmp = new link(a, b);
						this->links.push_back(tmp);
						return true;
					}
				}
				return false;
			}

			inline const state& get_now_state() const noexcept
			{
				return *now_state;
			}

			inline constexpr void add_state(state *new_state) noexcept
			{
				if(new_state != nullptr)
				{
					states.push_back(new_state);
				}
			}

			inline const void add_state(const std::string state_name) noexcept
			{
				if(get_state(state_name) != nullptr)
				{
					state *tmp = new state(state_name);
					states.push_back(tmp);
				}
			}

			inline bool delete_state(state * state) noexcept
			{
				if(state != nullptr)
				{
					states.remove(state);
					return true;
				}
				return false;
			}

			inline bool delete_state(std::string name) noexcept
			{
				if(delete_state(const_cast<state*>(get_state(name))))
				{
					return true;
				}
				return false;
			}

			inline const link* const get_link(std::string state1, std::string state2) const noexcept
			{
				const state *tmp = get_state(state1);
				const state *tmp2 = get_state(state2);
				if(tmp != nullptr && tmp2 != nullptr)
				{
					for(auto i : links)
					{
						if(i->get_current_state()->get_name() == tmp->get_name() && i->get_current_state()->get_name() == tmp2->get_name())
						{
							return i;
						}
					}
				}
				return nullptr;
			}

			inline bool delete_link(std::string state1, std::string state2) noexcept
			{
				auto t = get_link(state1, state2);
				if(t != nullptr)
				{
					links.remove(const_cast<link*>(t));
					return true;
				}
				return false;
			}

			inline bool change_link(std::string state1, std::string state2, unsigned int op) const noexcept
			{
				const state *tmp = get_state(state1);
				const state *tmp2 = get_state(state2);
				if(tmp != nullptr && tmp2 != nullptr)
				{
					for(auto i : links)
					{
						if(i->get_current_state()->get_name() == tmp->get_name()
						   && i->get_next_state()->get_name() == tmp2->get_name())
						{
							i->set_ops(0);
						}
					}
					return true;
				}
				return false;
			}

			inline void update(float delta) noexcept
			{
				simulate(delta);
			}

			~map()
			{
				if(now_state != nullptr)
				{
					delete now_state;
				}

				states.clear();
				links.clear();
			}

		};
	}
}


