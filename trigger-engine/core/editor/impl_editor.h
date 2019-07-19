#ifndef EDITOR_H
#define EDITOR_H
#include "../../imgui/imgui.h"
#include "../game/transform.h"

namespace trigger::edit
{
	class impl_editor : public trigger::transform
	{
	public:
		// Using imgui Draw module.
		// engine->editors
		virtual bool draw() noexcept = 0;

		// Update editor vals
		virtual void update(float delta) noexcept = 0;
	};
} // namespace trigger::edit

#endif