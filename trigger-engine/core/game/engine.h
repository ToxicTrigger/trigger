#ifndef ENGINE_H
#define ENGINE_H

#include "../../renderer/renderer.h"
#include "world.h"
#include "mini_core.h"
#include "pool.h"
#include "../core/editor/main_editor.h"

#ifdef _WIN64
#include <windows.h>
#endif

//MOVE pool.h

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
			trigger::tools::pool<trigger::transform> pool;
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